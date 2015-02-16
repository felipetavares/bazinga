#include "spacer.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Spacer::Spacer (Type type) {
	this->type = type;
}

void gui::Spacer::event (Event& evt) {

}

void gui::Spacer::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Spacer::getPreferredSize (int& w, int& h) {
	switch (type) {
		case BOTH:
			h = w = ANY;
		break;
		case VERTICAL:
			w = 1;
			h = ANY;
		break;
		case HORIZONTAL:
			w = ANY;
			h = HORIZONTAL;
		break;
	}
}

void gui::Spacer::getMinSize (int& w, int& h) {
	h = w = 1;
}

void gui::Spacer::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Spacer::render (int x, int y) {
}

int gui::Spacer::getW () {
	return w;
}

int gui::Spacer::getH () {
	return h;
}
