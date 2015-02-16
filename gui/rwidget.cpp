#include "rwidget.h"
using namespace bazinga;

gui::ResponsiveWidget::ResponsiveWidget () {
	outside = false;
}

void gui::ResponsiveWidget::event (Event& evt) {
	if (inside(evt.x, evt.y, ux, uy, w, h)) {
		switch (evt.type) {
			case Event::MOUSEPRESS:
				ractivate(evt.x-ux, evt.y-uy);
			break;
			case Event::MOUSEUNPRESS:
				rclick(evt.x-ux, evt.y-uy);
			break;
			case Event::MOUSEMOVE:
				if (outside)
					renter(evt.x-ux, evt.y-uy);
				rmove(evt.x-ux, evt.y-uy);
				outside = false;
			break;
		}

		evt.invalidate();
	} else {
		if (!outside)
			rleave(evt.x-ux, evt.y-uy);
		outside = true;
	}
}

void gui::ResponsiveWidget::render (int x, int y) {
	ux = this->x+x;
	uy = this->y+y;

	rrender(x, y);
}

void gui::ResponsiveWidget::rleave (int x, int y) {

}

void gui::ResponsiveWidget::rclick (int x, int y) {
	
}

void gui::ResponsiveWidget::renter (int x, int y) {

}

void gui::ResponsiveWidget::ractivate (int x, int y) {
	
}

void gui::ResponsiveWidget::rmove (int x, int y) {
	
}