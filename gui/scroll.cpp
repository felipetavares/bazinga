#include "scroll.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Scroll::Scroll () {
	position = 0.5;
	handleSize = 24;
	drag = -1;
}

void gui::Scroll::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Scroll::getPreferredSize (int& w, int& h) {
	w = ANY;
	h = 12;
}

void gui::Scroll::getMinSize (int& w, int& h) {
	w = 48;
	h = 12;
}

void gui::Scroll::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Scroll::rrender (int x, int y) {
	video::setColor(1, 1, 1, 0.5);
	video::fillRect(this->x+x, this->y+y, w, h);

	int start = position*(w-handleSize);
	video::setColor(0.4, 0.7, 0.9, 1);
	video::fillRect(this->x+x+start, this->y+y, handleSize, h);
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
}

void gui::Scroll::ractivate (int x, int y) {
	int start = position*(w-handleSize);
	if (x >= start && x <= start+handleSize) { // Clicked over the handle
		drag = x-start;
	}
}

void gui::Scroll::rmove (int x, int y) {
	if (drag > 0) {
		position = float(x-drag)/float(w-handleSize);
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