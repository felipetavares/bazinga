#ifndef BAZINGA_GUI_ENTRY_H
#define BAZINGA_GUI_ENTRY_H

#include "../gui.h"
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class Entry :public ResponsiveWidget {
			int cam;

			int cursor;
			string text;
			bool focused;
			float timeSinceLastType;
		public:
			function <void(Widget*)> onChange;

			Entry(string="");

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			int getW ();
			int getH ();
			string getText();
			void setText(string);
			void rrender(int, int);
			void rleave(int, int);
			//void renter(int, int);
			void rclick(int, int);
			void rkey(string, string);
			//void ractivate(int, int);
			void focus();
			void unfocus();
		private:
			void insert(string);
			void remove();
			void move(bool);
		};
	}
}

#endif /* BAZINGA_GUI_ENTRY_H */