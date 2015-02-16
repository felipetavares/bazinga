#ifndef BAZINGA_GUI_BUTTON_H
#define BAZINGA_GUI_BUTTON_H

#include "../gui.h"

namespace bazinga {
	namespace gui {
		class Button :public Widget {
			string text;
		public:
			Button(string);

			void event (Event&);
			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			void render(int, int);
			int getW ();
			int getH ();
		};
	}
}

#endif /* BAZINGA_GUI_BUTTON_H */