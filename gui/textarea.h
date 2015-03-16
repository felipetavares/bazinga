#ifndef BAZINGA_TEXTAREA_H
#define BAZINGA_TEXTAREA_H

#include "../gui.h"
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class TextArea :public ResponsiveWidget {
			string text;
			bool focused;

			vector<string> screen;
		public:
			TextArea(string="");

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
			void X(int, int);
		};
	}
}

#endif /* BAZINGA_TEXTAREA_H */