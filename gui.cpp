#include "gui.h"
#include "video.h"
#include "cache.h"
#include <sstream>
using namespace std;
using namespace bazinga;

vector <gui::Window*> gui::windows;

gui::Container::Container (Flow flow):
	x(0), y(0), w(0), h(0),
	borderLeft(6), borderRight(6), borderTop(6), borderBottom(6),
	spacing(6) {
	this->flow = flow;
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

void gui::Container::getPreferredSize (int &pw, int &ph) {
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

void gui::Container::getMinSize (int &sX, int &sY) {
	sX = 0;
	sY = 0;

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

	for (auto child :children) {
		child->render(wx, wy);
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

gui::Window::Window (string title, int w, int h) {
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

	// In the future: add "&& thisWindowIsFocused"
	if (inside(evt.x, evt.y, x, y, w, h)) {
		//evt.invalidate();
	}

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
}

void gui::init () {
}

void gui::render () {
	// Render all windows
	for (int w=0;w<windows.size();w++) {
		if (windows[w]->close) {
			delete windows[w];
			windows.erase(windows.begin()+w);
		} else {
			windows[w]->render();
		}
	}
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

void gui::mousemove (int x, int y) {
	Event event = Event(Event::MOUSEMOVE);
	event.x = x;
	event.y = y;

	for (auto w :windows) {
		w->event(event);

		if (!event.isValid())
			break;
	}	
}

void gui::mousepress (int button, int x, int y) {
	Event event = Event(Event::MOUSEPRESS);
	event.button = button;
	event.x = x;
	event.y = y;

	for (auto w :windows) {
		w->event(event);

		if (!event.isValid())
			break;
	}	
}

void gui::mouseunpress (int button, int x, int y) {
	Event event = Event(Event::MOUSEUNPRESS);
	event.button = button;
	event.x = x;
	event.y = y;

	for (auto w :windows) {
		w->event(event);

		if (!event.isValid())
			break;
	}	
}

bool gui::inside (int px, int py,
				  int bx, int by, int bw, int bh) {
  bw += bx;
  bh += by;

  return px >= bx && px <= bw &&
         py >= by && py <= bh;
}