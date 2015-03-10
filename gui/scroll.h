#ifndef BAZINGA_GUI_SCROLL_H
#define BAZINGA_GUI_SCROLL_H

#include "../gui.h"
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class Scroll :public ResponsiveWidget {
			float position;
			int handleSize;
			int drag;
			Container::Flow flow;
		public:
			function <void(Widget*)> onChange;

			Scroll(Container::Flow);

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			int getW ();
			int getH ();

			void rrender(int, int);
			void rmove(int, int);
			void rleave(int, int);
			//void renter(int, int);
			void rclick(int, int);
			void ractivate(int, int);

			float getScroll();
		};
	}
}

#endif /* BAZINGA_GUI_SCROLL_H */