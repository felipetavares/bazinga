#include "checkbox.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::CheckBox::CheckBox (string text) {
	this->text = text;
	click = false;
	hover = false;
	image = NULL;

	checked = false;
}

gui::CheckBox::CheckBox (Path path) {
	click = false;
	hover = false;
	text = "";
	image = cache::getTexture(path);

	checked = false;
}

void gui::CheckBox::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::CheckBox::getPreferredSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText(text);

	if (text == "") {
		w = 16+12+16;
	} else {
		w = size.w+12+16;
	}

	h = 16+12;
}

void gui::CheckBox::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText(text);

	w = size.w+12;
	h = 16+12;
}

void gui::CheckBox::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::CheckBox::rrender (int x, int y) {
	//video::fillVGradient(this->x+x, this->y+y, w, h);

	if (image) {
		image->render(ux+w/2, uy+h/2);
	}

	auto font = cache::getFont("default");
	font->setColor(*gui::colors["text.regular"]);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(text, this->x+x+(this->w-16)/2+16, this->y+y+this->h/2);

	if (click) {
		video::setColor1(*gui::colors["active"]);
	} else if (hover) {
		video::setColor1(*gui::colors["foreground"]);
	}
	else {
		video::setColor1(*gui::colors["text.regular"]);
	}

	video::strokeRect(ux, uy+6, 16, 16);

	if (checked) {
		cache::getTexture(Path("assets/gui/check.png"))->render(ux+8, uy+14);
	}
}

int gui::CheckBox::getW () {
	return w;
}

int gui::CheckBox::getH () {
	return h;
}

void gui::CheckBox::rleave (int x, int y) {
	hover = false;
	click = false;
}

void gui::CheckBox::renter (int x, int y) {
	hover = true;
}

void gui::CheckBox::rclick (int x, int y) {
	click = false;

	checked = !checked;

	if (onClick)
		onClick(this);
}

void gui::CheckBox::ractivate (int x, int y) {
	click = true;
}

bool gui::CheckBox::isChecked () {
	return checked;
}

void gui::CheckBox::setChecked (bool checked) {
	this->checked = checked;
}