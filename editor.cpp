#include "editor.h"

#include "gui.h"
#include "gui/label.h"
#include "gui/entry.h"
#include "gui/button.h"
#include "gui/spacer.h"
#include "gui/bgraph.h"
#include "gui/scroll.h"
#include "gui/filemanager.h"
#include "gui/progress.h"
#include "math/vec2/vec2.h"
#include "console.h"
#include "map.h"
#include <cmath>
#include <sstream>
using namespace bazinga;

gui::Window *editor::currentPropWindow = NULL;

void editor::openNewSceneWindow () {
	auto window = new gui::Window("Nova Cena", 300, 300);

	auto container = new gui::Container(gui::Container::VERTICAL);

	auto line1 = new gui::Container(gui::Container::HORIZONTAL);
	auto line2 = new gui::Container(gui::Container::HORIZONTAL);
	auto line3 = new gui::Container(gui::Container::HORIZONTAL);

	auto sceneName = new gui::Entry("SemTitulo");
	line1->add(new gui::Label("Nome: "));
	line1->add(sceneName);

	auto completePath = new gui::Label("");
	line2->add(completePath);

	auto cancelButton = new gui::Button("Cancelar");
	auto createButton = new gui::Button("Criar");
	line3->add(cancelButton);
	line3->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	line3->add(createButton);

	container->add(line1);
	container->add(line2);
	container->add(new gui::Spacer(gui::Spacer::VERTICAL));
	container->add(line3);

	window->setRoot(container);
	gui::add(window);

	sceneName->onChange = [completePath,sceneName] (gui::Widget *wid) {
		//completePath->setText(sceneName->getText());
	};

	sceneName->onChange(sceneName);

	window->onUpdate = [=] (gui::Window* win) {
	};

	window->onClose = [] (gui::Window* win) {
	};

	cancelButton->onClick = [window] (gui::Widget *wid) {
		window->close = true;
	};

	createButton->onClick = [=] (gui::Widget* wid) {
		if (getActiveMap())
			delete getActiveMap();
		activeMap = new Map(Path("scenes/"+sceneName->getText()+".scene"));
		activeMap->init();
		window->close = true;
	};
}

void editor::openProgressWindow (float *p) {
	auto window = new gui::Window("Salvando", 300, 90);

	auto container = new gui::Container(gui::Container::VERTICAL);

	auto progess = new gui::Progress(0);
	container->add(progess);

	window->setRoot(container);
	gui::add(window);

	window->onUpdate = [=] (gui::Window* win) {
		progess->setPosition(*p);

		if (*p >= 1) {
			win->close = true;
			delete p;
		}
	};

	window->onClose = [] (gui::Window* win) {
	};
}

void editor::openPropertiesWindow (Object *o) {
	gui::Window *window;
	string windowTitle = "Objeto: "+o->str_properties["name"];

	if (currentPropWindow != NULL) {
		window = currentPropWindow;
		window->setTitle(windowTitle);
	} else {
		window = new gui::Window(windowTitle, 400, video::windowHeight);
	}

	auto container = new gui::Container(gui::Container::VERTICAL);
	auto buttonsContainer = new gui::Container(gui::Container::HORIZONTAL, false, true);
	auto viewContainer = new gui::Container(gui::Container::HORIZONTAL);
	auto propertiesContainer = new gui::Container (gui::Container::VERTICAL, true);

	container->add(viewContainer);
	container->add(buttonsContainer);

	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	auto newButton = new gui::Button("Nova");
	buttonsContainer->add(newButton);
	auto applyButton = new gui::Button("Aplicar");
	buttonsContainer->add(applyButton);
	auto fileButton = new gui::Button("Arquivo");
	buttonsContainer->add(fileButton);
	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	buttonsContainer->borderLeft = buttonsContainer->borderRight = 0;
	buttonsContainer->borderTop = buttonsContainer->borderBottom = 0;

	viewContainer->add(propertiesContainer);
	auto scrollBar = new gui::Scroll(gui::Container::VERTICAL);
	viewContainer->add(scrollBar);
	viewContainer->borderLeft = viewContainer->borderRight = 0;
	viewContainer->borderTop = viewContainer->borderBottom = 0;

	for (auto entry :o->num_properties) {
		stringstream sstream;
		sstream << entry.second;

		auto info = new gui::Container(gui::Container::HORIZONTAL,false,true);
		info->add(new gui::Entry(entry.first));
		info->add(new gui::Entry(sstream.str()));
		auto del = new gui::Button(Path("assets/gui/delete.png"));
		info->add(del);
		del->onClick = [=] (gui::Widget* w) {
			propertiesContainer->remove(info);
		};
		propertiesContainer->add(info);
	}

	for (auto entry :o->str_properties) {
		auto info = new gui::Container(gui::Container::HORIZONTAL,false,true);
		info->add(new gui::Entry(entry.first));
		info->add(new gui::Entry(entry.second));
		auto del = new gui::Button(Path("assets/gui/delete.png"));
		info->add(del);
		del->onClick = [=] (gui::Widget* w) {
			propertiesContainer->remove(info);
		};
		propertiesContainer->add(info);
	}

	propertiesContainer->borderLeft = propertiesContainer->borderRight = 0;
	propertiesContainer->borderTop = propertiesContainer->borderBottom = 0;

	window->setRoot(container);

	if (window != currentPropWindow)
		gui::add(window, video::windowWidth/2-400, numeric_limits<int>::max());

	window->onUpdate = [=] (gui::Window* win) {
	};

	window->onClose = [=] (gui::Window* win) {
		currentPropWindow = NULL;
	};

	scrollBar->onChange = [=] (gui::Widget* wid) {
		propertiesContainer->scrollV(scrollBar->getScroll());
	};

	newButton->onClick = [=] (gui::Widget* w) {
		auto info = new gui::Container(gui::Container::HORIZONTAL,false,true);
		info->add(new gui::Entry("nova"));
		info->add(new gui::Entry());
		auto del = new gui::Button(Path("assets/gui/delete.png"));
		info->add(del);
		del->onClick = [=] (gui::Widget* w) {
			propertiesContainer->remove(info);
		};
		propertiesContainer->add(info);
	};

	applyButton->onClick = [=] (gui::Widget *w) {
		o->num_properties.clear();
		o->str_properties.clear();

		for (auto wid :propertiesContainer->children) {
			auto c = (gui::Container*)wid;
			string key = ((gui::Entry*)c->children[0])->getText();
			string value = ((gui::Entry*)c->children[1])->getText();

			try {
				float fvalue = stof(value);
				o->num_properties[key] = fvalue;
			} catch (exception e) {
				o->str_properties[key] = value;
			}
		}
	};

	fileButton->onClick = [=] (gui::Widget* w) {
		new gui::FileManager(NULL);
	};

	currentPropWindow = window;
}

editor::Editor::Editor () {
	enableGrid = false;
	dragging = false;
	gridPosition = {0, 0};
	gridSize = {80, 80};
}

void editor::Editor::select (Object *obj) {
	if (input::keyboard["space"])
		return;

	if (obj == NULL) {
		selection.clear();
		dragStart.clear();
		return;
	}

	if (find(selection.begin(), selection.end(), obj) != selection.end())
		return;

	if (!input::keyboard["left shift"]) {
		selection.clear();
		dragStart.clear();
	}
	selection.push_back(obj);
	dragStart.push_back(vec2());
}

void editor::Editor::drag (vec2 position) {
	int i = 0;
	if (dragging) {
		if (input::keyboard["space"]) {
			getActiveMap()->camx = (int)(position*-1-camDragStart).x;
			getActiveMap()->camy = (int)(position*-1-camDragStart).y;
			return;
		}

		if (dragType == 0) {
			for (auto o :selection) {
				o->num_properties["w"] = (int)(position-dragStart[i]).x;
				o->num_properties["h"] = (int)(position-dragStart[i]).y;
				i++;
			}
		} else if (dragType == 1) {
			for (auto o :selection) {
				o->num_properties["gx"] = (int)(position-dragStart[i]).x;
				o->num_properties["gy"] = (int)(position-dragStart[i]).y;
				i++;
			}
		} else {
			for (auto o :selection) {
				o->num_properties["x"] = (int)(position-dragStart[i]).x;
				o->num_properties["y"] = (int)(position-dragStart[i]).y;
				i++;
			}
		}

		srectSize = position-srectPosition;
	}
}
void editor::Editor::setDrag (bool dragging, vec2 dragStart) {
	this->dragging = dragging;

	if (input::keyboard["space"]) {
		camDragStart = vec2(-getActiveMap()->camx,
							-getActiveMap()->camy)-dragStart;
		return;
	}

	if (dragging == true) {
		srectPosition = dragStart;
		srectSize = vec2(0,0);

		int i = 0;
		for (auto o :selection) {
			if (input::keyboard["left ctrl"]) {
				dragType = 0;
				this->dragStart[i] = dragStart-vec2(o->num_properties["w"],
													o->num_properties["h"]);
			} else if (input::keyboard["left alt"]) {
				dragType = 1;
				this->dragStart[i] = dragStart-vec2(o->num_properties["gx"],
													o->num_properties["gy"]);
			} else {
				dragType = 2;
				this->dragStart[i] = dragStart-vec2(o->num_properties["x"],
													o->num_properties["y"]);
			}
			i++;
		}
	} else {
		for (auto o :selection)
			toGrid(o);
		if (selection.size() == 0) {
			this->dragStart.clear();
			for (auto o :getActiveMap()->objects) {
				int x = srectPosition.x;
				int y = srectPosition.y;
				int w = srectSize.x;
				int h = srectSize.y;

				int ox = o->num_properties["x"]-getActiveMap()->camx;
				int oy = o->num_properties["y"]-getActiveMap()->camy;
				int ow = o->num_properties["w"];
				int oh = o->num_properties["h"];

				if (gui::insideRect(ox, oy, ow, oh, x, y, w, h)) {
					selection.push_back(o);
					this->dragStart.push_back(vec2());
				}
			}
		}
	}
}

void editor::Editor::render () {
	int i = 0;
	for (auto o :selection) {
		if (i == selection.size()-1)
			o->renderSelected(0);
		else
			o->renderSelected(1);
		i++;
	}

	glPushMatrix();
	glLoadIdentity();

	float zoomX = getActiveMap()->zoomX;
	float zoomY = getActiveMap()->zoomY;

	if (enableGrid) {
		float dX = gridSize.x*zoomX;
		float dY = gridSize.y*zoomY;

		float sX = -fmod(getActiveMap()->camx*zoomX,gridSize.x*zoomX)-((int)(video::windowWidth/2/(gridSize.x*zoomX))*(gridSize.x*zoomX));
		int top = -video::windowHeight/2;
		int h = video::windowHeight;

		float sY = -fmod(getActiveMap()->camy*zoomY,gridSize.y*zoomY)-((int)(video::windowHeight/2/(gridSize.y*zoomY))*(gridSize.y*zoomY));
		int left = -video::windowWidth/2;
		int w = video::windowWidth;

		for (float x=sX;x<video::windowWidth;x+=dX) {
			video::setColor1(*gui::colors["foreground"]);
			video::fillRect(x, top, 1, h);
		}
		for (float y=sY;y<video::windowHeight;y+=dY) {
			video::setColor1(*gui::colors["foreground"]);
			video::fillRect(left, y, w, 1);
		}
	}

	if (dragging && selection.size() == 0) {
		video::setColor1(*gui::colors["foreground"]);
		video::fillRect(srectPosition.x*zoomX, srectPosition.y*zoomY, srectSize.x*zoomX, srectSize.y*zoomY);
	}

	glPopMatrix();
}

void editor::Editor::toGrid (Object *o) {
	if (!enableGrid)
		return;

	/* Vectors something like
	 this
	   \ /
	    #
	   / \
	*/
	vec2 q1 {sqrt(2),sqrt(2)};
	vec2 q2 {-sqrt(2),sqrt(2)};
	vec2 q3 {-sqrt(2),-sqrt(2)};
	vec2 q4 {sqrt(2),-sqrt(2)};

	// Object's position
	vec2 op = {
		o->num_properties["x"],
		o->num_properties["y"]
	};
	// Grid's center
	vec2 cp = {
		op.x-fmod((op.x-gridPosition.x),gridSize.x)+gridSize.x/2,
		op.y-fmod((op.y-gridPosition.y),gridSize.y)+gridSize.y/2,
	};

	// Direction vector
	vec2 d = cp-op;
	d.normalize();

	// Calculate the dot products
	float l[4];
	vec2 q[4] = {q1, q2, q3, q4};
	for (int i=0;i<4;i++) {
		l[i] = q[i].dot(d);
	}

	// Find the smallest dot product
	int smallest = 0;
	for (int i=1;i<4;i++) {
		if (l[i] < l[smallest]) {
			smallest = i;
		}
	}

	switch (smallest) {
		case 0:
			toGrid1(o);
		break;
		case 1:
			toGrid2(o);
		break;
		case 2:
			toGrid3(o);
		break;
		case 3:
			toGrid4(o);
		break;
	}
}

void editor::Editor::toGrid1 (Object *o) {
	auto x = o->num_properties["w"]/2;
	auto y = o->num_properties["h"]/2;

	auto px = o->num_properties["x"];
	auto py = o->num_properties["y"];
	auto pw = o->num_properties["w"];
	auto ph = o->num_properties["h"];

	o->num_properties["x"] = x + px - fmod((px+x-gridPosition.x),gridSize.x)+gridPosition.x - pw;
	o->num_properties["y"] = y + py - fmod((py+y-gridPosition.y),gridSize.y)+gridSize.y - ph;
}

void editor::Editor::toGrid2 (Object *o) {
	auto x = o->num_properties["w"]/2;
	auto y = o->num_properties["h"]/2;

	auto px = o->num_properties["x"];
	auto py = o->num_properties["y"];
	//auto pw = o->num_properties["w"];
	auto ph = o->num_properties["h"];

	o->num_properties["x"] = x + px - fmod((px+x-gridPosition.x),gridSize.x);
	o->num_properties["y"] = y + py - fmod((py+y-gridPosition.y),gridSize.y)+gridSize.y - ph;
}

void editor::Editor::toGrid3 (Object *o) {
	auto x = o->num_properties["w"]/2;
	auto y = o->num_properties["h"]/2;

	auto px = o->num_properties["x"];
	auto py = o->num_properties["y"];
	//auto pw = o->num_properties["w"];
	//auto ph = o->num_properties["h"];

	o->num_properties["x"] = x + px - fmod((px+x-gridPosition.x),gridSize.x);
	o->num_properties["y"] = y + py - fmod((py+y-gridPosition.y),gridSize.y);
}

void editor::Editor::toGrid4 (Object *o) {
	auto x = o->num_properties["w"]/2;
	auto y = o->num_properties["h"]/2;

	auto px = o->num_properties["x"];
	auto py = o->num_properties["y"];
	auto pw = o->num_properties["w"];
	//auto ph = o->num_properties["h"];

	o->num_properties["x"] = x + px - fmod((px+x-gridPosition.x),gridSize.x)+gridSize.x - pw;
	o->num_properties["y"] = y + py - fmod((py+y-gridPosition.y),gridSize.y);
}

void editor::Editor::setGrid (bool enable) {
	enableGrid = enable;
}

vec2 editor::Editor::getGridSize() {
	return gridSize;
}

void editor::Editor::setGridSize(vec2 size) {
	if (size.x > 0 && size.y > 0)
		gridSize = size;
}

void editor::Editor::copyObject () {
	for (auto o :selection) {
		getActiveMap()->addObject(new Object(*o));
	}
}

void editor::Editor::deleteObject () {
	for (auto o :selection) {
		getActiveMap()->deleteObject((int)(long)o);
	}

	selection.clear();
}
