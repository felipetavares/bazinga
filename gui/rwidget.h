#ifndef BAZINGA_GUI_RWIDGET_H
#define BAZINGA_GUI_RWIDGET_H

#include "../gui.h"

namespace bazinga {
	namespace gui {
		class ResponsiveWidget :public Widget {
			bool outside;
		public:
			ResponsiveWidget();

			void event (Event&);
			virtual void pack (int, int) = 0;
			virtual void getPreferredSize(int&, int&) = 0;
			virtual void getMinSize(int&, int&) = 0;
			virtual void setPosition(int, int) = 0;
			virtual void render(int, int);
			virtual int getW () = 0;
			virtual int getH () = 0;
			
			virtual void rrender(int, int) = 0;
			virtual void renter(int, int);
			virtual void rleave(int, int);
			virtual void ractivate(int, int);
			virtual void rclick(int, int);
			virtual void rmove(int, int);
			virtual void rkey(string);
		};
	}
}

#endif /* BAZINGA_GUI_RWIDGET_H */