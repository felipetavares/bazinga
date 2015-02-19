#include "gui.h"
#include "video.h"
#include "cache.h"
#include <sstream>
using namespace std;
using namespace bazinga;

gui::Scissor gui::scissor = Scissor(0,0,0,0);
stack <gui::Scissor> gui::scissors;
vector <gui::Window*> gui::windows;
gui::Widget* gui::focus;

void gui::Widget::focus () {

}

void gui::Widget::unfocus () {

}

gui::Scissor::Scissor (int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

gui::Scissor gui::Scissor::operator+ (const Scissor& other) {
	Scissor final = Scissor(0,0,0,0);

	if (other.x > x) {
		final.x = other.x;
	} else {
		final.x = x;
	}

	if (other.x+other.w < x+w) {
		final.w = other.x+other.w-final.x;
	} else {
		final.w = x+w-final.x;
	}

	if (other.y > y) {
		final.y = other.y;
	} else {
		final.y = y;
	}

	if (other.y+other.h < y+h) {
		final.h = other.y+other.h-final.y;
	} else {
		final.h = y+h-final.y;
	}

	if (final.w < 0 or final.h < 0)
		final = *this;

	return final;
}

void gui::Scissor::apply () {
	// As all operations in the stencil are done like:
	// op & mask, if we set the mask to 0xff, the op
	// will be applied, so this is like "enable stencil drawing"
	glStencilMask(0xff);
	// Disable writing on the color buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	// Make the test always fail
	glStencilFunc (GL_NEVER, 1, 0xff);
	// Now, everywhere we draw, the stencil buffer will be set, because:
	// stencil = ref & mask = 1 (and also because the test will aways fail)
	// and so this will aways be executed
	glStencilOp (GL_REPLACE, GL_KEEP, GL_KEEP);
	// Now, clear the stencil
	glClear(GL_STENCIL_BUFFER_BIT);

	// Set the drawable area
	video::fillRect(x-1, y-1, w+2, h+2);

	// This is like "disable stencil drawing"
	glStencilMask(0x00);
	// Enable the color buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	// Only draw in the places where the stencil is set
	glStencilFunc(GL_EQUAL, 1, 0xff);
}

gui::Container::Container (Flow flow, bool scrollable):
	x(0), y(0), w(0), h(0),
	borderLeft(6), borderRight(6), borderTop(6), borderBottom(6),
	spacing(6) {
	this->flow = flow;
	this->scrollable = scrollable;
	scrollX = scrollY = 0;
}

void gui::Container::event (Event& evt) {
	if (evt.isValid()) {
		int i=0;
		while (evt.isValid() && i<children.size()) {
			children[i]->event(evt);
			i++;
		}
	}
}

void gui::Container::pack (int sX, int sY) {
	int usedX=0, usedY=0;
	int freeX=0, freeY=0;
	int anynX=0, anynY=0;
	int minsX, minsY;
	getMinSize(minsX, minsY);
	getFullSize(fullW, fullH);

	int posX = x+borderLeft;
	int posY = y+borderTop;

	if (sX < minsX)
		sX = minsX;
	if (sY < minsY)
		sY = minsY;

	for (auto child :children) {
		int csizeX,csizeY;
		child->getPreferredSize(csizeX, csizeY);

		if (csizeX == ANY)
			anynX++;
		if (csizeY == ANY)
			anynY++;

		if (flow == VERTICAL) {
			usedX = csizeX>usedX?csizeX:usedX;
			usedY += csizeY;
		} else {
			usedX += csizeX;
			usedY = csizeY>usedY?csizeY:usedY;
		}
	}

	if (flow == VERTICAL) {
		freeX = sX-(borderRight+borderLeft);
		freeY = sY-usedY-spacing*(children.size()-1)
				-(borderTop+borderBottom);
	} else {
		freeX = sX-usedX-spacing*(children.size()-1)
				-(borderLeft+borderRight);
	}

	if (freeX > 0 && freeY > 0) {
		w = sX;
		h = sY;
	} else {
		w = sX-(freeX<0?freeX:0);
		h = sY-(freeY<0?freeY:0);
	}

	for (auto child :children) {
		int csizeX, csizeY;
		int rsizeX, rsizeY;
		child->getPreferredSize(csizeX, csizeY);

		if (flow == VERTICAL) {
			if (csizeX == ANY && csizeY == ANY) {
				rsizeX = freeX;
				rsizeY = freeY/anynY;
			} else if (csizeX == ANY) {
				rsizeX = freeX;
				rsizeY = csizeY;
			} else if (csizeY == ANY) {
				rsizeX = csizeX;
				rsizeY = freeY/anynY;
			} else {
				rsizeX = csizeX;
				rsizeY = csizeY;
			}
		} else {
			if (csizeX == ANY && csizeY == ANY) {
				rsizeX = freeX/anynX;
				rsizeY = freeY;
			} else if (csizeX == ANY) {
				rsizeX = freeX/anynX;
				rsizeY = csizeY;
			} else if (csizeY == ANY) {
				rsizeX = csizeX;
				rsizeY = freeY;
			} else {
				rsizeX = csizeX;
				rsizeY = csizeY;
			}
		}

		child->setPosition(posX, posY);
		child->pack(rsizeX, rsizeY);

		if (flow == VERTICAL) {
			posY += rsizeY+spacing;
		} else {
			posX += rsizeX+spacing;
		}
	}
}

void gui::Container::getFullSize (int &pw, int &ph) {
	pw = ph = -1;

	for (auto child :children) {
		int csizeX, csizeY;
		child->getPreferredSize(csizeX, csizeY);

		if (csizeX == ANY)
			pw = csizeX;
		if (csizeY == ANY)
			ph = csizeY;

		if (flow == VERTICAL) {
			if (pw != ANY && csizeX != ANY) {
				pw = csizeX>pw?csizeX:pw;
			}
			if (ph != ANY && csizeY != ANY) {
				ph += csizeY+spacing;
			}
		} else {
			if (ph != ANY && csizeY != ANY) {
				ph = csizeY>ph?csizeY:ph;
			}
			if (pw != ANY && csizeX != ANY) {
				pw += csizeX+spacing;
			}
		}
	}
}

void gui::Container::getPreferredSize (int &pw, int &ph) {
	pw = -1;
	ph = -1;

	if (scrollable) {
		pw = ANY;
		ph = ANY;
		return;
	}

	for (auto child :children) {
		int csizeX, csizeY;
		child->getPreferredSize(csizeX, csizeY);

		if (csizeX == ANY)
			pw = csizeX;
		if (csizeY == ANY)
			ph = csizeY;

		if (flow == VERTICAL) {
			if (pw != ANY && csizeX != ANY) {
				pw = csizeX>pw?csizeX:pw;
			}
			if (ph != ANY && csizeY != ANY) {
				ph += csizeY+spacing;
			}
		} else {
			if (ph != ANY && csizeY != ANY) {
				ph = csizeY>ph?csizeY:ph;
			}
			if (pw != ANY && csizeX != ANY) {
				pw += csizeX+spacing;
			}
		}
	}
}

void gui::Container::getMinSize (int &sX, int &sY) {
	sX = 0;
	sY = 0;

	if (scrollable) {
		sX = 48;
		sY = 48;
		return;
	}

	for (auto child :children) {
		int csizeX, csizeY;
		child->getMinSize(csizeX, csizeY);

		if (flow == VERTICAL) {
			sX = csizeX>sX?csizeX:sX;
			sY += csizeY+spacing;
		} else {
			sY = csizeY>sY?csizeY:sY;
			sX += csizeX+spacing;
		}
	}

	sX += borderLeft+borderRight;
	sY += borderTop+borderBottom;

	if (sX == 0)
		sX = 1;
	if (sY == 0)
		sY = 1;
}

void gui::Container::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Container::render (int wx, int wy) {
	//video::setColor (1, 0, 0, 1);
	//video::fillRect (wx, wy, w, h);

	stringstream titleStream;
	titleStream << fullW << " / ";
	titleStream << w << " / ";
	titleStream << scrollable;

	auto font = cache::getFont("default");
	font->setColor(0,0,0,1);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Middle);
	text::fillText(titleStream.str(), wx+x, wy+y);

	for (auto child :children) {
		child->render(wx-scrollX*(fullW-w), wy+scrollY);
	}
}

void gui::Container::add (Widget *widget) {
	children.push_back(widget);
}

int gui::Container::getW () {
	return w;
}

int gui::Container::getH () {
	return h;
}

void gui::Container::scrollH (float scrollX) {
	this->scrollX = scrollX;
}

gui::Event::Event (Type type) {
	this->type = type;
	captured = false;
}

bool gui::Event::isValid () {
	return !captured;
}

void gui::Event::invalidate () {
	captured = true;
}

gui::Window::Window (string title, int w, int h):
	onUpdate(NULL) {
	this->title = title;
	this->w = w;
	this->h = h;

	// Center
	x = -w/2;
	y = -h/2;

	// Default: 24
	tbar = 24;

	// Grabbing and resizing
	grabbing = false;
	resizing = false;

	// Mouse over buttons?
	overclose = false;
	overmaximize = false;

	// Grab Vector
	gvecx = 0;
	gvecy = 0;

	// Close window
	close = false;

	// Root widget
	root = NULL;
}

void gui::Window::setRoot (Widget *root) {
	this->root = root;
	this->root->pack(w,h-tbar-12);
	w = root->getW();
	h = root->getH()+tbar+12;
}

void gui::Window::event (Event& evt) {
	if (!evt.isValid())
		return;

	switch (evt.type) {
		case Event::MOUSEPRESS:
		    if (!grabbing && inBar(evt.x, evt.y)) {
				gvecx = evt.x-x;
				gvecy =  evt.y-y;
				grabbing = true;
		    }

		    if (!resizing && inResize(evt.x,evt.y)) {
				gvecx = evt.x-this->w;
				gvecy = evt.y-this->h;
				resizing = true;
		    }

		    if (inClose(evt.x, evt.y)) {
				close = true;
		    }
		    if (inMaximize(evt.x, evt.y)) {
				x = -video::windowWidth/2;
				y = -video::windowHeight/2;
				w = video::windowWidth;
				h = video::windowHeight;
				if (root)
					root->pack(w,h-tbar-12);
				w = root->getW();
				h = root->getH()+tbar+12;
		    }
			if (inside(evt.x, evt.y, x, y, w, h)) {
				bringFront(this);
			}
		break;
		case Event::MOUSEUNPRESS:
		    this->grabbing = false;
		    this->resizing = false;
		break;
		case Event::MOUSEMOVE:
		    overclose = inClose(evt.x, evt.y);
		    overmaximize = inMaximize(evt.x, evt.y);

		    if (grabbing) {
				x = evt.x-gvecx;
				y = evt.y-gvecy;
				evt.invalidate();
		    }
		    if (resizing) {
				w = evt.x-gvecx;
				h = evt.y-gvecy;

				if (root)
					root->pack(w,h-tbar-12);

				w = root->getW();
				h = root->getH()+tbar+12;
				evt.invalidate();
		  	}
		break;
	}

	if (evt.isValid()) {
		if (root && inside(evt.x, evt.y, x, y+tbar, w, h-12-tbar))
			root->event(evt);
	}

	if (inside(evt.x, evt.y, x, y, w, h)) {
		evt.invalidate();
	}
}

bool gui::Window::inClose (int x, int y) {
	int cx = this->x+tbar/2-x;
	int cy = this->y+tbar/2-y;
	int r = tbar/4;

	return cx*cx+cy*cy < r*r;
}

bool gui::Window::inMaximize (int x, int y) {
	int cx = this->x+tbar/2*3-x;
	int cy = this->y+tbar/2-y;
	int r = tbar/4;

	return cx*cx+cy*cy < r*r;
}

bool gui::Window::inBar (int x, int y) {
    return inside (x, y,
    			   this->x, this->y, this->w, this->tbar);
}

bool gui::Window::inResize (int x, int y) {
	return inside (x, y,
				   this->x+this->w-12,
				   this->y+this->h-12,
				   12,
				   12);
}

void gui::Window::render () {
	onUpdate(this);

	save();
	combineScissor(Scissor(x, y, w, h));

	video::setColor(0.5, 0.8, 1, 1);
	video::fillRect(x, y, w, h);

	video::setColor(1, 1, 1, 1);
	video::fillRect(x, y, w, tbar);

	video::setColor(0.8,0,0,1);
	video::fillCircle(x+tbar/2,y+tbar/2,tbar/4);

	video::setColor(0.3,0.8,0.2,1);
	video::fillCircle(x+tbar/2*3,y+tbar/2,tbar/4);

	auto font = cache::getFont("default");
	font->setColor(0,0,0,1);
	font->setSize(tbar/4*3);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(title, this->x+this->w/2, this->y+tbar/2);

	if (overclose) {
		auto close = cache::getTexture(Path("assets/gui/close.png"));
		close->render(x+tbar/2, y+tbar/2);
	}

	if (overmaximize) {
		auto maximize = cache::getTexture(Path("assets/gui/maximize.png"));
		maximize->render(x+tbar/2*3, y+tbar/2);
	}

	auto resize = cache::getTexture(Path("assets/gui/resize.png"));
	resize->render(this->x+this->w-6, this->y+this->h-6);

	if (root) {
		root->render(x, y+tbar);
	}

	video::setColor(0.2, 0.2, 0.2, 1);
	video::strokeRect(x, y, w, h);

	restore();
}

void gui::init () {
	focus = NULL;
}

void gui::render () {
	// Enable stencil test
	glEnable(GL_STENCIL_TEST);

	setScissor(Scissor(-video::windowWidth/2,
					   -video::windowHeight/2,
					   video::windowWidth,
					   video::windowHeight));

	// Render all windows
	for (int w=0;w<windows.size();w++) {
		if (windows[w]->close) {
			delete windows[w];
			windows.erase(windows.begin()+w);
		} else {
			windows[w]->render();
		}
	}

	// Disable stencil test
	glDisable (GL_STENCIL_TEST);
}

void gui::deinit () {
	// Delete all windows
	for (auto w :windows) {
		delete w;
	}
}

void gui::add (gui::Window* window) {
	windows.push_back(window);
}

void gui::setFocus (Widget* wid) {
	if (focus)
		unsetFocus(focus);
	focus = wid;
	focus->focus();
}

void gui::unsetFocus (Widget* wid) {
	if (focus == wid) {
		focus->unfocus();
		focus = NULL;
	}
}

void gui::bringFront (Window* window) {
	for (int i=0;i<windows.size();i++) {
		if (windows[i] == window) {
			if (i == windows.size()-1)
				return;
			windows.erase(windows.begin()+i);
			break;
		}
	}

	windows.push_back(window);
}

void gui::mousemove (int x, int y) {
	Event event = Event(Event::MOUSEMOVE);
	event.x = x;
	event.y = y;

	for (int w=windows.size()-1;w>=0;w--) {
		windows[w]->event(event);

		if (!event.isValid())
			break;
	}
}

void gui::mousepress (int button, int x, int y) {
	Event event = Event(Event::MOUSEPRESS);
	event.button = button;
	event.x = x;
	event.y = y;

	for (int w=windows.size()-1;w>=0;w--) {
		windows[w]->event(event);

		if (!event.isValid())
			break;
	}
}

void gui::mouseunpress (int button, int x, int y) {
	Event event = Event(Event::MOUSEUNPRESS);
	event.button = button;
	event.x = x;
	event.y = y;

	for (int w=windows.size()-1;w>=0;w--) {
		windows[w]->event(event);

		if (!event.isValid())
			break;
	}
}

void gui::keypress (uint16_t unicode) {
	if (focus) {
		Event evt = Event(Event::KEYPRESS);
		evt.unicode = unicode;
		focus->event(evt);
	}
}

bool gui::inside (int px, int py,
				  int bx, int by, int bw, int bh) {
  bw += bx;
  bh += by;

  return px >= bx && px <= bw &&
         py >= by && py <= bh;
}

void gui::setScissor (Scissor sc) {
	scissor = sc;
	scissor.apply();
}

void gui::combineScissor (Scissor sc) {
	scissor = scissor+sc;
	scissor.apply();
}

void gui::save() {
	scissors.push(scissor);
}

void gui::restore () {
	auto sc = scissors.top();
	scissors.pop();
	setScissor(sc);
}
