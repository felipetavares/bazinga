#include "slider.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Slider::Slider (float position) {
	this->position = position;
	active = false;
	onChange = NULL;
}

void gui::Slider::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Slider::getPreferredSize (int& w, int& h) {
	w = ANY;
	h = 12;
}

void gui::Slider::getMinSize (int& w, int& h) {
	w = 12;
	h = 12;
}

void gui::Slider::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Slider::rrender (int x, int y) {
	video::setColor(1, 1, 1, 0.5);
	video::fillRect(this->x+x, this->y+y+3, w, 6);
	video::setColor(0.4, 0.7, 0.9, 1);
	video::fillCircle(this->x+x+w*position, this->y+y+6, 6);
}

int gui::Slider::getW () {
	return w;
}

int gui::Slider::getH () {
	return h;
}

float gui::Slider::getPosition () {
	return position;
}

void gui::Slider::setPosition (float position) {
	this->position = position;
}

void gui::Slider::rmove (int x, int y) {
	if (active) {
		position = float(x)/float(w);
		onChange(this);
	}
}

void gui::Slider::ractivate(int x, int y) {
	position = float(x)/float(w);	
	onChange(this);	

	active = true;
}

void gui::Slider::rclick (int x, int y) {
	active = false;
}

void gui::Slider::rleave (int x, int y) {
	active = false;
}