#ifndef BAZINGA_GUI_BUTTON_H
#define BAZINGA_GUI_BUTTON_H

#include "../gui.h"
// Full-featured event-enabled widget
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class Button :public ResponsiveWidget {
			string text;
			bool click;
			bool hover;
		public:
			function <void(Widget*)> onClick;

			Button(string);

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			int getW ();
			int getH ();

			void rrender(int, int);
			void rleave(int, int);
			void renter(int, int);
			void rclick(int, int);
			void ractivate(int, int);
		};
	}
}

#endif /* BAZINGA_GUI_BUTTON_H */