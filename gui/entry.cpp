#include "entry.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Entry::Entry () {
	this->text = "";
	focused = false;
}

void gui::Entry::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Entry::getPreferredSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText("#");

	w = ANY;
	h = size.h+12;
}

void gui::Entry::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText("#");

	w = size.w+12;
	h = size.h+12;
}

void gui::Entry::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Entry::rrender (int x, int y) {
	if (focused)
		video::setColor(0.9, 0.7, 0.4, 1); // Click color
	//else if (hover)
	//	video::setColor(0.4, 0.7, 0.9, 1); // Hover color
	else
		video::setColor(1, 1, 1, 0.5);

	video::fillRect(this->x+x, this->y+y, w, h);

	auto font = cache::getFont("default");
	font->setColor(0,0,0,1);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Alphabetic);
	text::fillText(text, this->x+x, this->y+y+this->h/2);
}

int gui::Entry::getW () {
	return w;
}

int gui::Entry::getH () {
	return h;
}

//void gui::Entry::rleave (int x, int y) {
//}

//void gui::Entry::renter (int x, int y) {
//}

void gui::Entry::focus () {
	focused = true;
}

void gui::Entry::unfocus () {
	focused = false;
}

void gui::Entry::rclick (int x, int y) {
	gui::setFocus(this);
}

//void gui::Entry::ractivate (int x, int y) {
//	click = true;
//}

void gui::Entry::rkey (string key) {
	text += key;
}