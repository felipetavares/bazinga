#include "button.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Button::Button (string text) {
	this->text = text;
	click = false;
	hover = false;
}

void gui::Button::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Button::getPreferredSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText(text);

	w = size.w+12;
	h = size.h+12;
}

void gui::Button::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText(text);

	w = size.w+12;
	h = size.h+12;
}

void gui::Button::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Button::rrender (int x, int y) {
	if (click)
		video::setColor(0.9, 0.7, 0.4, 1); // Click color
	else if (hover)
		video::setColor(0.4, 0.7, 0.9, 1); // Hover color
	else
		video::setColor(1, 1, 1, 0.5);

	video::fillRect(this->x+x, this->y+y, w, h);

	auto font = cache::getFont("default");
	font->setColor(0,0,0,1);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(text, this->x+x+this->w/2, this->y+y+this->h/2);
}

int gui::Button::getW () {
	return w;
}

int gui::Button::getH () {
	return h;
}

void gui::Button::rleave (int x, int y) {
	hover = false;
	click = false;
}

void gui::Button::renter (int x, int y) {
	hover = true;
}

void gui::Button::rclick (int x, int y) {
	click = false;

	if (onClick)
		onClick(this);
}

void gui::Button::ractivate (int x, int y) {
	click = true;
}