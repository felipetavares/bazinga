#ifndef BAZINGA_GUI_SPACER_H
#define BAZINGA_GUI_SPACER_H

#include "../gui.h"

namespace bazinga {
	namespace gui {
		class Spacer :public Widget {
		public:
			enum Type {VERTICAL, HORIZONTAL, BOTH};
		private:
			Type type;
		public:
			Spacer(Type);

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

#endif /* BAZINGA_GUI_SPACER_H */