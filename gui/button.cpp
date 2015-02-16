#include "button.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Button::Button (string text) {
	this->text = text;
}

void gui::Button::event (Event& evt) {

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

void gui::Button::render (int x, int y) {
	video::setColor(1, 1, 1, 1);
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
