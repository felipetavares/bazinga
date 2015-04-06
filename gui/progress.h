#ifndef BAZINGA_GUI_PROGRESS_H
#define BAZINGA_GUI_PROGRESS_H

#include "../gui.h"

namespace bazinga {
	namespace gui {
		class Progress :public Widget {
			float position;
		public:
			Progress(float);

			void event (Event&);
			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			void render(int, int);
			int getW ();
			int getH ();

			void setPosition(float);
			float getPosition();
		};
	}
}

#endif /* BAZINGA_GUI_PROGRESS_H */