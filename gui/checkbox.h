#ifndef BAZINGA_GUI_CHECKBOX_H
#define BAZINGA_GUI_CHECKBOX_H

#include "../gui.h"
// Full-featured event-enabled widget
#include "rwidget.h"
#include "../image.h"

namespace bazinga {
	namespace gui {
		class CheckBox :public ResponsiveWidget {
			Image *image;
			string text;
			bool click;
			bool hover;
			bool checked;
		public:
			function <void(Widget*)> onClick;

			CheckBox(string);
			CheckBox(Path);

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

			bool isChecked();
			void setChecked(bool);
		};
	}
}

#endif /* BAZINGA_GUI_CHECKBOX_H */