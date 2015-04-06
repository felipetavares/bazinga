#ifndef BAZINGA_GUI_LABEL_H
#define BAZINGA_GUI_LABEL_H

#include "../gui.h"
#include "../image.h"

namespace bazinga {
	namespace gui {
		class Label :public Widget {
			string text;
			Image *image;
		public:
			Label(string);
			Label(Path);

			void setText(string);

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

#endif /* BAZINGA_GUI_LABEL_H */