#include "bgraph.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::BGraph::BGraph () {
	max = 0;
	size = 3;
}

void gui::BGraph::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::BGraph::getPreferredSize (int& w, int& h) {
	w = 1000;
	h = 200;
}

void gui::BGraph::getMinSize (int& w, int& h) {
	w = 12;
	h = 12;
}

void gui::BGraph::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::BGraph::rrender (int x, int y) {
	video::setColor(1, 1, 1, 0.5);
	video::fillRect(this->x+x, this->y+y, w, h);

	float pX = this->x+x;
	float pY = this->y+y+h;
	float avg = 0;
	video::setColor(0.4, 0.7, 0.9, 1); // Hover color

	float newmax = 0;

	for (float bar :bars) {
		if (bar > newmax)
			newmax = bar;

		video::fillRect(pX,pY-h*(bar/max),size,h*(bar/max));
		avg += bar/float(bars.size());
		pX += size;
	}

	stringstream ss;
	ss << avg;
	avg = this->y+y+h-h*(avg/max);

	video::setColor(1.0, 0.0, 0.0, 1);
	video::fillRect(this->x+x,avg,w,1);

	auto font = cache::getFont("default");
	font->setColor(0,0,0,1);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Center);
	text::setBaseline(text::Middle);
	text::fillText(ss.str(), this->x+x+this->w/2, avg);

	max = newmax;
}

void gui::BGraph::addBar (float bar) {
	bars.push_back(bar);

	while ((bars.size()-1)*size>w) {
		bars.erase(bars.begin());
	}
}

int gui::BGraph::getW () {
	return w;
}

int gui::BGraph::getH () {
	return h;
}
