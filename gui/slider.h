#ifndef BAZINGA_GUI_SLIDER_H
#define BAZINGA_GUI_SLIDER_H

#include "../gui.h"
#include "rwidget.h"
#include <functional>
using namespace std;

namespace bazinga {
	namespace gui {
		class Slider :public ResponsiveWidget {
			float position;
			bool active;
		public:
			function <void(Widget*)> onChange;

			Slider(float);

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			void rrender(int, int);
			int getW ();
			int getH ();

			void setPosition(float);
			float getPosition();

			void ractivate(int, int);
			void rmove(int, int);
			void rclick(int, int);
			void rleave(int, int);
		};
	}
}

#endif /* BAZINGA_GUI_SLIDER_H */