#include "scroll.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Scroll::Scroll (Container::Flow flow) {
	this->flow = flow;
	position = 0;
	handleSize = 24;
	drag = -1;
}

void gui::Scroll::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Scroll::getPreferredSize (int& w, int& h) {
	if (flow == Container::HORIZONTAL) {
		w = ANY;
		h = 12;
	} else {
		w = 12;
		h = ANY;
	}
}

void gui::Scroll::getMinSize (int& w, int& h) {
	if (flow == Container::HORIZONTAL) {
		w = 48;
		h = 12;
	} else {
		w = 12;
		h = 48;
	}
}

void gui::Scroll::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Scroll::rrender (int x, int y) {
	video::setColor1(*gui::colors["background"]);
	video::fillRect(this->x+x, this->y+y, w, h);

	int start = flow==Container::HORIZONTAL?position*(w-handleSize):position*(h-handleSize);
	video::setColor1(*gui::colors["foreground"]);
	
	if (flow == Container::HORIZONTAL) 
		video::fillRect(this->x+x+start, this->y+y, handleSize, h);
	else
		video::fillRect(this->x+x, this->y+y+start, w, handleSize);
}

int gui::Scroll::getW () {
	return w;
}

int gui::Scroll::getH () {
	return h;
}

void gui::Scroll::rleave (int x, int y) {
	drag = -1;
}

//void gui::Scroll::renter (int x, int y) {
//}

void gui::Scroll::rclick (int x, int y) {
	drag = -1;
	unsetMouseFocus(this);
}

void gui::Scroll::ractivate (int x, int y) {
	setMouseFocus(this);

	int start = flow==Container::HORIZONTAL?position*(w-handleSize):position*(h-handleSize);

	if (flow == Container::HORIZONTAL) {
		if (x >= start && x <= start+handleSize) { // Clicked over the handle
			drag = x-start;
		}
	} else {
		if (y >= start && y <= start+handleSize) { // Clicked over the handle
			drag = y-start;
		}
	}
}

void gui::Scroll::rmove (int x, int y) {
	if (drag > 0) {
		if (flow == Container::HORIZONTAL)
			position = float(x-drag)/float(w-handleSize);
		else
			position = float(y-drag)/float(h-handleSize);

		if (position < 0)
			position = 0;
		if (position > 1)
			position = 1;
		
		if (onChange)
			onChange(this);
	}
}

float gui::Scroll::getScroll () {
	return this->position;
}