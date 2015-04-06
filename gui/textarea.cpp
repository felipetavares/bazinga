#include "textarea.h"

#include "textarea.h"
#include "../cache.h"
#include "../text.h"
#include "../math/vec2/vec2.h"

using namespace bazinga;

gui::TextArea::TextArea (string text) {
	this->text = text;
	focused = false;

	X(0, text.size());
}

void gui::TextArea::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::TextArea::getPreferredSize (int& w, int& h) {
	w = ANY;
	h = ANY;
}

void gui::TextArea::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText("#");

	w = size.w+12;
	h = size.h+12;
}

void gui::TextArea::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::TextArea::rrender (int x, int y) {
	save();
	combineScissor(Scissor(ux, uy, w-1, h-1), true);

	video::setColor1(video::Color(1, 1, 1, 0.5));
	video::fillRect(ux, uy, w, h);

	if (focused) {
		int shadowSize = 2;
		video::Color c = *gui::colors["active"];
		video::setColor1(video::Color(c.r, c.g, c.b, 0.5));
		video::setColor2(video::Color(c.r, c.g, c.b, 0));
		video::shadow(ux+shadowSize, uy+shadowSize, w-2*shadowSize, h-2*shadowSize, shadowSize);
	}

	auto font = cache::getFont("default");
	font->setColor(*gui::colors["text.regular"]);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Top);

	vec2 tPos = {0, 0};
	for (auto l :screen) {
		text::fillText(l, ux, uy+tPos.y);
		tPos.y += text::measureText(l).h+2; // Spacing of 2 px between lines
	}

	restore();
}

int gui::TextArea::getW () {
	return w;
}

int gui::TextArea::getH () {
	return h;
}

void gui::TextArea::rleave (int x, int y) {
	//gui::unsetFocus(this);
}

//void gui::TextArea::renter (int x, int y) {
//}

void gui::TextArea::focus () {
	focused = true;
}

void gui::TextArea::unfocus () {
	focused = false;
}

void gui::TextArea::rclick (int x, int y) {
	gui::setFocus(this);
}

//void gui::TextArea::ractivate (int x, int y) {
//	click = true;
//}

void gui::TextArea::rkey (string value, string key) {
	if (key == "left shift" ||
		key == "right shift" ||
		key == "left ctrl" ||
		key == "right ctrl" ||
		key == "left alt" ||
		key == "right alt" ||
		key == "left super" ||
		key == "right super" ||
		key == "numlock") {
		return;
	}

	if (key == "backspace") {
		//remove();
	} else if (key == "right") {
		//move(true);
	} else if (key == "left") {
		//move(false);
	} else {
		//insert(value);
	}
}

string gui::TextArea::getText () {
	return text;
}

void gui::TextArea::setText (string text) {
	this->text = text;
}

void gui::TextArea::X (int s, int e) {
	screen.clear();
	string buffer;

	for (int p=s;p<e;p++) {
		if (text[p] == '\n') {
			screen.push_back(buffer);
			buffer = "";
		} else {
			buffer += text[p];
		}
	}

	if (buffer != "")
		screen.push_back(buffer);
}