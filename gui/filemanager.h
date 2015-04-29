#ifndef BAZINGA_FILE_MANAGER_H
#define BAZINGA_FILE_MANAGER_H

#include "../gui.h"
#include "entry.h"

namespace bazinga {
	namespace gui {
		class FileManager {
			gui::Window* window;
			gui::Container* container;
			gui::Container* filesContainer;
			gui::Entry *finalEntry;
			Path currentDir;
		public:
			function <void(Path)> onSelect;

			FileManager(Entry*,string="Arquivos",string="Abrir");

			void openDirectory(Path);
		private:
		};
	}
}

#endif /* BAZINGA_FILE_MANAGER_H */
