#include "entry.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Entry::Entry (string text) {
	this->text = text;
	focused = false;
	cursor = text.size();
	cam = 0;
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
	//save();
	//combineScissor(Scissor(ux, uy, w-1, h-1));

	video::setColor1(video::Color(0, 0, 0, 0));
	video::setColor2(video::Color(0, 0, 0, 0.2));
	video::shadow(ux, uy, w, h, 1);

	timeSinceLastType += bazinga::delta;

	video::setColor1(video::Color(1, 1, 1, 0.5));
	video::fillRect(this->x+x, this->y+y, w, h);

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
	text::setBaseline(text::Middle);

	int n;
	for (n=cam+w/12;n<text.size();n++)
		if (text::measureText(text.substr(cam, n)).w > w)
			break;

	text::fillText(text.substr(cam, n), this->x+x, this->y+y+this->h/2);

	int cursorX = ux+text::measureText(text.substr(cam, cursor)).w;

	if ((int(bazinga::curtime)%2 == 0 ||
		timeSinceLastType < 0.5) && focused) {
		video::setColor1(*gui::colors["text.regular"]);
		video::fillRect(cursorX,uy+2,
						1,h-4);
	}

	if (text::measureText(text.substr(cam, cursor)).w > w) {
		cam++;
		cursor--;
	}

	//restore();
}

int gui::Entry::getW () {
	return w;
}

int gui::Entry::getH () {
	return h;
}

void gui::Entry::rleave (int x, int y) {
	//gui::unsetFocus(this);
}

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

void gui::Entry::rkey (string value, string key) {
	timeSinceLastType = 0;

	if (key == "left shift" ||
		key == "right shift" ||
		key == "left ctrl" ||
		key == "right ctrl" ||
		key == "left alt" ||
		key == "right alt" ||
		key == "left super" ||
		key == "right super" ||
		key == "numlock" ||
		key == "down" ||
		key == "up") {
		return;
	}

	if (key == "backspace") {
		remove();
	} else if (key == "right") {
		move(true);
	} else if (key == "left") {
		move(false);
	} else {
		insert(value);
	}
}

void gui::Entry::insert (string data) {
	text = text.substr(0, cam+cursor)+data+text.substr(cam+cursor, text.size());
	cursor ++;

	if (onChange)
		onChange(this);
}

void gui::Entry::remove () {
	if (text.size() > 0) {
		text = text.substr(0, cam+cursor-1)+text.substr(cam+cursor, text.size());	
		cursor--;

		if (cursor < 0) {
			cursor ++;
			cam --;
		}
	}

	if (onChange)
		onChange(this);
}

void gui::Entry::move (bool dir) {
	if (dir) {
		if (cam+cursor < text.size())
			cursor++;
	} else {
		if (cam+cursor > 0)
			cursor--;
		if (cursor < 0) {
			cursor ++;
			cam --;
		}
	}
}

string gui::Entry::getText () {
	return text;
}

void gui::Entry::setText (string text) {
	this->text = text;
	cursor = text.size();
}