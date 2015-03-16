#include "rwidget.h"
using namespace bazinga;

gui::ResponsiveWidget::ResponsiveWidget () {
	outside = false;
}

void gui::ResponsiveWidget::event (Event& evt) {
	if (evt.type != Event::KEYPRESS) {
		if (inside(evt.x, evt.y, ux, uy, w, h) ||
			mouseFocus == this) {
			switch (evt.type) {
				case Event::MOUSEPRESS:
					ractivate(evt.x-ux, evt.y-uy);
					evt.invalidate();
				break;
				case Event::MOUSEUNPRESS:
					rclick(evt.x-ux, evt.y-uy);
					evt.invalidate();
				break;
				case Event::MOUSEMOVE:
					if (outside)
						renter(evt.x-ux, evt.y-uy);
					rmove(evt.x-ux, evt.y-uy);
					outside = false;
					//evt.invalidate();
				break;
			}
		} else {
			if (!outside)
				rleave(evt.x-ux, evt.y-uy);
			outside = true;
		}
	} else {
		uint16_t unicode = evt.unicode;

		string utf8 = "";

		if ((unicode & 0xFF80) == 0) { // 1 byte UTF-8
			utf8 += unicode & 0x7F;
		} else if (unicode & 0xF800) { // 2 byte UTF-8
			//utf8 += unicode & 0x7F;
			//utf8 += (unicode & 0x7FF) >> 8;
		}

		rkey(utf8, evt.keyname);

		evt.invalidate();
	}
}

void gui::ResponsiveWidget::render (int x, int y) {
	ux = this->x+x;
	uy = this->y+y;

	//save();
	//combineScissor(Scissor(ux, uy, w-1, h-1));
	rrender(x, y);
	//restore();
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

void gui::ResponsiveWidget::rdisable (int x, int y) {
	
}

void gui::ResponsiveWidget::rkey (string keyValue, string keyName) {
	
}