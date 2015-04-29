#include "paletteitem.h"
#include "../cache.h"
#include "../text.h"
using namespace bazinga;

gui::PaletteItem::PaletteItem (string text, vector<int> visualEd) {
	this->text = text;
	this->visualEd = visualEd;
	//image = NULL;
  selected = false;
}

/*
gui::PaletteItem::PaletteItem (Path path) {
	text = "";
	image = cache::getTexture(path);
}
*/

void gui::PaletteItem::setText (string text, vector <int> visualEd) {
	this->text = text;
	this->visualEd = visualEd;
}

void gui::PaletteItem::event (Event& evt) {

}

void gui::PaletteItem::pack (int w, int h) {
	this->w = w;
	this->h = h;
}

void gui::PaletteItem::getPreferredSize (int& w, int& h) {
	/*
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Top);
	auto size = text::measureText(text);
	*/

	w = ANY;
	h = 36;
}

void gui::PaletteItem::getMinSize (int& w, int& h) {
	auto font = cache::getFont("default");
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Top);
	auto size = text::measureText(text);

	w = size.w+12;
	h = size.h+36;
}

void gui::PaletteItem::setPosition (int x, int y) {
	this->x = x;
	this->y = y;
}

void gui::PaletteItem::render (int x, int y) {
  video::setColor1(*gui::colors[selected?"foreground":"background.contrast"]);
  video::fillRect(this->x+x, this->y+y, w, h);
	video::setColor1(*gui::colors["background.contrast.extra"]);
	video::strokeRect(this->x+x, this->y+y, w, h);

	auto font = cache::getFont("default");
	font->setColor(*gui::colors["text.regular"]);
	font->setSize(16);
	text::setFont(font);
	text::setAlign(text::Left);
	text::setBaseline(text::Middle);
	text::fillArgText(text, this->x+x, this->y+y+this->h/2,
										visualEd, vector <function<void(text::Font*)>> {
												[] (text::Font *f) {
													f->setColor(*gui::colors["text.regular"]);
												},
												[] (text::Font *f) {
													f->setColor(*gui::colors["text.contrast"]);
												}
										});

  /*
	if (image) {
		image->render(this->x+x+this->w/2, this->y+y+this->h/2);
	}
  */
}

int gui::PaletteItem::getW () {
	return w;
}

int gui::PaletteItem::getH () {
	return h;
}

void gui::PaletteItem::select (bool selected) {
  this->selected = selected;
}
