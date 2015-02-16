#ifndef BAZINGA_GUI_H
#define BAZINGA_GUI_H

#include "bazinga.h"
#include <vector>
using namespace std;
using namespace bazinga;

namespace bazinga {
	namespace gui {

#define ANY 0

		class Event {
			bool captured;
		public:
			enum Type {MOUSEPRESS, MOUSEUNPRESS, MOUSEMOVE};

			Type type;
			int button;
			int x, y;

			Event (Type);
			bool isValid();
			void invalidate();
		};

		class Widget {
		protected:
			int x, y;
			int w, h;
		public:
			virtual void event (Event&) = 0;
			virtual void pack (int, int) = 0;
			virtual void getPreferredSize(int&, int&) = 0;
			virtual void getMinSize(int&, int&) = 0;
			virtual void setPosition(int, int) = 0;
			virtual void render(int, int) = 0;
			virtual int getW () = 0;
			virtual int getH () = 0;
		};

		// The heart of any dynamic GUI:
		// the container.	
		class Container :public Widget {
		public:
			enum Flow {VERTICAL, HORIZONTAL};

			int spacing;
			int borderLeft, borderRight;
			int borderTop, borderBottom;
		private:
			Flow flow;

			int x, y;
			int w, h;

			vector <Widget*> children;
		public:
			Container(Flow);

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void setPosition(int, int);
			void render(int, int);
			void add(Widget*);
			void event(Event&);
			int getW();
			int getH();
		};

		class Window {
			int x, y;
			int w, h;
			int tbar;

			bool grabbing;
			bool resizing;
			bool overclose;
			bool overmaximize;

			int gvecx, gvecy;

			string title;

			Widget* root;
		public:
			bool close;

			Window(string, int, int);

			void event(Event&);
			void setRoot (Widget*);
			void render();
		private:
			bool inBar(int, int);
			bool inResize(int, int);
			bool inClose(int, int);
			bool inMaximize(int, int);
		};

		extern vector <Window*> windows;

		void init();
		void render();
		void deinit();

		// Add a new window to the interface
		void add(Window*);

		// Handle mouse events
		void mousemove (int, int);
		void mousepress (int, int, int);
		void mouseunpress (int);
	
		bool inside(int, int,
					int, int, int, int);
	}
}


#endif /* BAZINGA_GUI_H */