#ifndef BAZINGA_GUI_BGRAPH_H
#define BAZINGA_GUI_BGRAPH_H

#include "../gui.h"
#include "rwidget.h"

namespace bazinga {
	namespace gui {
		class BGraph :public ResponsiveWidget {
			vector <float> bars;
			float max;
			float size;
		public:
			BGraph();

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			int getW ();
			int getH ();

			void rrender(int, int);
			void addBar(float);
		};
	}
}

#endif /* BAZINGA_GUI_BGRAPH_H */