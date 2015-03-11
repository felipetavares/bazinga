#include "entry.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::Entry::Entry () {
	this->text = "";
	focused = false;
	cursor = 0;
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
	save();
	combineScissor(Scissor(ux, uy, w-1, h-1));

	timeSinceLastType += bazinga::delta;

	if (focused)
		video::setColor1(video::Color(0.9, 0.7, 0.4, 1)); // Click color
	//else if (hover)
	//	video::setColor1(video::Color(0.4, 0.7, 0.9, 1)); // Hover color
	else
		video::setColor1(video::Color(1, 1, 1, 0.5));

	video::fillRect(this->x+x, this->y+y, w, h);

	auto font = cache::getFont("default");
	font->setColor(video::Color(0,0,0,1));
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Middle);
	text::fillText(text.substr(cam, text.size()), this->x+x, this->y+y+this->h/2);


	int cursorX = ux+text::measureText(text.substr(cam, cursor)).w;

	if (int(bazinga::curtime*4)%2 == 0 ||
		!focused ||
		timeSinceLastType < 0.5) {
		video::setColor1(video::Color(0,0,0,1));
		video::fillRect(cursorX,uy,
						1,h);
	}

	if (text::measureText(text.substr(cam, cursor)).w > w) {
		cam++;
		cursor--;
	}

	restore();
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

void gui::Entry::rkey (string value, string key) {
	timeSinceLastType = 0;

	if (key == "left shift" ||
		key == "right shift" ||
		key == "left ctrl" ||
		key == "right ctrl") {
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