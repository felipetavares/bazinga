#include "spacer.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Spacer::Spacer () {
}

void gui::Spacer::event (Event& evt) {

}

void gui::Spacer::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Spacer::getPreferredSize (int& w, int& h) {
	h = w = ANY;
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
