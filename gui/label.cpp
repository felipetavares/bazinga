#include "label.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Label::Label (string text) {
	this->text = text;
}

void gui::Label::setText (string text) {
	this->text = text;
}

void gui::Label::event (Event& evt) {

}

void gui::Label::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Label::getPreferredSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Top);
	auto size = text::measureText(text);

	w = size.w+12;
	h = size.h+12;
}

void gui::Label::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Top);
	auto size = text::measureText(text);

	w = size.w+12;
	h = size.h+12;
}

void gui::Label::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Label::render (int x, int y) {
	auto font = cache::getFont("default");
	font->setColor(*gui::colors["text.regular"]);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(text, this->x+x+this->w/2, this->y+y+this->h/2);
}

int gui::Label::getW () {
	return w;
}

int gui::Label::getH () {
	return h;
}
