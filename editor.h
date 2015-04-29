#ifndef BAZINGA_EDITOR_H
#define BAZINGA_EDITOR_H

#include "gui.h"
#include "gui/paletteitem.h"
#include "math/vec2/vec2.h"

namespace bazinga {
	namespace editor {
		class Command;

		extern vector <Command*> commands;

		extern gui::Window *currentPropWindow;
		void openPropertiesWindow(Object*);
		void openProgressWindow(float*);
		void openNewSceneWindow();
		void openCommandPaletteWindow();

		/*
			Command

			Represents a command that can be used through the
			command palette.
		*/
		class Command {
			/*
				The command's name used to search & display

				Must be in the format: module.command, e.g.:

					* bazinga.reload
					* bazinga.projectpath.set

				Note that there's no real single way to represent a
				command, just try to make the best choice and you are
				fine.
			*/
			string name;
			/*
				Function that executes the respective command
			*/
			function <void(void)> func;

			string text;
		public:
			gui::PaletteItem *item;

			Command(string, function<void(void)>);

			/*
				Fuzzy string matching
			*/
			float score();
			float score(string);
			bool match(string);
			/*
				Executes this command
			*/
			void exec();

			string getName();
		};

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

		void init();
		void deinit();

		// Exposed API
		void createNewScene();
	}
}

#endif /* BAZINGA_EDITOR_H */
