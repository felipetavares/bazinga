#ifndef BAZINGA_GUI_PALETTEITEM_H
#define BAZINGA_GUI_PALETTEITEM_H

#include "../gui.h"
#include "../image.h"

namespace bazinga {
	namespace gui {
		class PaletteItem :public Widget {
			string text;
			vector <int> visualEd;
			//Image *image;
      bool selected;
		public:
			PaletteItem(string, vector<int>);
			//PaletteItem(Path);

			void setText(string, vector<int>);

			void event (Event&);
			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			void render(int, int);
			int getW ();
			int getH ();

      void select(bool);
		};
	}
}

#endif /* BAZINGA_GUI_PALETTEITEM_H */
