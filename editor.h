#ifndef BAZINGA_EDITOR_H
#define BAZINGA_EDITOR_H

#include "gui.h"
#include "math/vec2/vec2.h"

namespace bazinga {
	namespace editor {
		extern gui::Window *currentPropWindow;
		void openPropertiesWindow(Object*);
		void openProgressWindow(float*);
		void openNewSceneWindow();

		class Editor {
			vector <Object*> selection;
			bool dragging;
			// Start drag position
			vector <vec2> dragStart;

			vec2 camDragStart;
		
			vec2 gridPosition;
			vec2 gridSize;

			bool enableGrid;

			int dragType;

			vec2 srectPosition;
			vec2 srectSize;
		public:
			Editor();

			void select(Object*);
			void render();

			void drag(vec2);
			void setDrag(bool, vec2={0,0});

			void setGrid(bool);

			void toGrid(Object*);

			vec2 getGridSize();
			void setGridSize(vec2);

			void copyObject();
			void deleteObject();
		private:
			void toGrid1(Object*);
			void toGrid2(Object*);
			void toGrid3(Object*);
			void toGrid4(Object*);
		};
	}
}

#endif /* BAZINGA_EDITOR_H */