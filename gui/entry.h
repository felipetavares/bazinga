#ifndef BAZINGA_GUI_ENTRY_H
#define BAZINGA_GUI_ENTRY_H

#include "../gui.h"
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class Entry :public ResponsiveWidget {
			string text;
			//bool click;
			//bool hover;
		public:
			Entry();

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			int getW ();
			int getH ();

			void rrender(int, int);
			//void rleave(int, int);
			//void renter(int, int);
			void rclick(int, int);
			void rkey(string);
			//void ractivate(int, int);
		};
	}
}

#endif /* BAZINGA_GUI_ENTRY_H */