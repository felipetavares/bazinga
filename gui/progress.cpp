#include "progress.h"
#include "../cache.h"
#include "../text.h"
#include <cmath>
#include <sstream>
using namespace bazinga;
using namespace std;

gui::Progress::Progress (float position) {
	this->position = position;
}

void gui::Progress::event (Event& evt) {

}

void gui::Progress::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::Progress::getPreferredSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText("100%");

	w = ANY;
	h = size.h+12;
}

void gui::Progress::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	auto size = text::measureText("100%");

	w = size.w+12;
	h = size.h+12;
}

void gui::Progress::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::Progress::render (int x, int y) {
	video::setColor1(video::Color(0, 0, 0, 0));
	video::setColor2(video::Color(0, 0, 0, 0.2));
	video::shadow(this->x+x, this->y+y, w, h, 1);

	video::setColor1(*gui::colors["background.contrast"]);
	video::setColor2(*gui::colors["background"]);
	video::fillVGradient(this->x+x, this->y+y, w, h);
	video::setColor1(*gui::colors["foreground"]);
	video::setColor2(*gui::colors["foreground.contrast"]);
	video::fillVGradient(this->x+x, this->y+y,
					w*position, h);

	stringstream percent;
	percent << floor(position*100) << "%";

	auto font = cache::getFont("default");
	font->setColor(*gui::colors["text.regular"]);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(percent.str(), this->x+x+this->w/2, this->y+y+this->h/2);
}

int gui::Progress::getW () {
	return w;
}

int gui::Progress::getH () {
	return h;
}

float gui::Progress::getPosition () {
	return position;
}

void gui::Progress::setPosition (float position) {
	this->position = position;
}