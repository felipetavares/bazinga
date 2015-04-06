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
		public:
			FileManager(Entry*);

			void openDirectory(Path);
		private:
		};
	}
}

#endif /* BAZINGA_FILE_MANAGER_H */