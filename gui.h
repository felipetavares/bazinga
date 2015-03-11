#ifndef BAZINGA_GUI_H
#define BAZINGA_GUI_H

#include "bazinga.h"
#include <vector>
#include <map>
#include <stack>
using namespace std;
using namespace bazinga;

namespace bazinga {
	namespace gui {

#define ANY 0

		class Event {
			bool captured;
		public:
			enum Type {MOUSEPRESS, MOUSEUNPRESS, MOUSEMOVE, KEYPRESS};

			Type type;
			int button;
			int x, y;
			uint16_t unicode;
			string keyname;

			Event (Type);
			bool isValid();
			void invalidate();
		};

		class Scissor {
		protected:
			int x, y;
			int w, h;
		public:
			Scissor(int, int, int, int);
			void apply();
			Scissor operator+ (const Scissor&);
		};

		class Widget {
		protected:
			int x, y;
			int w, h;
			int ux, uy;
		public:
			virtual void event (Event&) = 0;
			virtual void pack (int, int) = 0;
			virtual void getPreferredSize(int&, int&) = 0;
			virtual void getMinSize(int&, int&) = 0;
			virtual void setPosition(int, int) = 0;
			virtual void render(int, int) = 0;
			virtual int getW () = 0;
			virtual int getH () = 0;
			virtual void focus();
			virtual void unfocus();
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

			float scrollX, scrollY;
			int x, y;
			int w, h;
			int fullW, fullH;

			vector <Widget*> children;
			bool scrollable;
			bool alignCenter;
		public:
			Container(Flow,bool=false,bool=false);

			void pack (int, int);
			void getPreferredSize(int&, int&);
			void getMinSize(int&, int&);
			void getFullSize(int&, int&);
			void setPosition(int, int);
			void render(int, int);
			void add(Widget*);
			void event(Event&);
			int getW();
			int getH();

			void scrollH(float);
			void scrollV(float);
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
			function <void (Window*)> onUpdate;

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

		extern Scissor scissor;
		extern stack <Scissor> scissors;
		extern vector <Window*> windows;
		extern map <string, video::Color*> colors;
		extern Widget* focus;
		extern Widget* mouseFocus;

		void init();
		void render();
		void deinit();

		// Add a new window to the interface
		void add(Window*);
		// Set/Unset focus
		void setFocus(Widget*); 
		void unsetFocus(Widget*);
		void setMouseFocus(Widget*);
		void unsetMouseFocus(Widget*);

		// Handle mouse events
		void mousemove (int, int);
		void mousepress (int, int, int);
		void mouseunpress (int, int, int);
		// Handle keyboard events
		void keypress(uint16_t, string);

		// Handy function
		bool inside(int, int,
					int, int, int, int);

		// Scissorssss, letsss CUTTTT
		void setScissor(Scissor, bool=true);
		void combineScissor(Scissor, bool=true);
		void save();
		void restore();

		// Window organization
		void bringFront(Window*);
	}
}


#endif /* BAZINGA_GUI_H */