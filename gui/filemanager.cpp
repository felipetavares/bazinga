#include "filemanager.h"
#include "label.h"
#include "entry.h"
#include "button.h"
#include "spacer.h"
#include "scroll.h"
#include "../console.h"
#include <sstream>
using namespace bazinga;

string toHumanSize (size_t size) {
	stringstream human;
	string sufix = "B";

	if (size > 1024) {
		size /= 1024;
		sufix = "kB";
	}

	if (size > 1024) {
		size /= 1024;
		sufix = "MB";
	}

	if (size > 1024) {
		size /= 1024;
		sufix = "GB";
	}

	human << size << sufix;

	return human.str();
}

gui::FileManager::FileManager (Entry *finalEntry, string title, string selectText):
 	onSelect(NULL) {
	this->finalEntry = finalEntry;

	window = new gui::Window(title, 800, 600);

	container = new gui::Container(gui::Container::VERTICAL);
	auto buttonsContainer = new gui::Container(gui::Container::HORIZONTAL, false, true);
	auto viewContainer = new gui::Container(gui::Container::HORIZONTAL);
	filesContainer = new gui::Container (gui::Container::VERTICAL, true);

	container->add(viewContainer);
	container->add(buttonsContainer);

	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	auto selectButton = new gui::Button(selectText);
	buttonsContainer->add(selectButton);
	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	buttonsContainer->borderLeft = buttonsContainer->borderRight = 0;
	buttonsContainer->borderTop = buttonsContainer->borderBottom = 0;

	viewContainer->add(filesContainer);
	auto scrollBar = new gui::Scroll(gui::Container::VERTICAL);
	viewContainer->add(scrollBar);
	viewContainer->borderLeft = viewContainer->borderRight = 0;
	viewContainer->borderTop = viewContainer->borderBottom = 0;

	selectButton->onClick = [=] (gui::Widget* wid) {
		if (onSelect) {
			onSelect(currentDir);
		}
	};

	scrollBar->onChange = [=] (gui::Widget* wid) {
		filesContainer->scrollV(scrollBar->getScroll());
	};

	filesContainer->borderLeft = filesContainer->borderRight = 0;
	filesContainer->borderTop = filesContainer->borderBottom = 0;

	currentDir = Path(".");
	openDirectory(currentDir);

	window->setRoot(container);
	window->onUpdate = [=] (gui::Window* win) {
	};
	gui::add(window);
}

void gui::FileManager::openDirectory (Path dir) {
	dir.normalize();

	currentDir = dir;

	console << LINEINFO << "opening directory: " << dir.getPath() << outline;

	bool listOk;
	auto directory = fs::listDirectory(dir, listOk);

	stable_sort(directory.begin(), directory.end(), [=] (Path a, Path b) {
		if (fs::isDir(a) && !fs::isDir(b))
			return true;
		if (fs::isDir(b) && !fs::isDir(a))
			return false;
		return a.getPath() < b.getPath();
	});

	filesContainer->clear();

	for (Path dentry :directory) {
		auto info = new gui::Container(gui::Container::HORIZONTAL,false,true);

		if (fs::isDir(dentry))
			info->add(new gui::Label(Path("assets/gui/folder.png")));
		else
			info->add(new gui::Label(Path("assets/gui/document.png")));

		auto enter = new gui::Button(dentry.getName());
		info->add(enter);
		info->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
		info->add(new gui::Label(toHumanSize(fs::getFileSize(dentry))));
		filesContainer->add(info);

		if (fs::isDir(dentry)) {
			enter->onClick = [=] (gui::Widget* w) {
				openDirectory(dentry);
			};
		} else {
			enter->onClick = [=] (gui::Widget* w) {
				if (finalEntry) {
					finalEntry->setText(Path(dentry).getPath());
				} else {
					if (gui::focus) {
						((gui::Entry*)gui::focus)->setText(Path(dentry).getPath());
					}
				}
				window->close = true;
			};
		}
	}
}
