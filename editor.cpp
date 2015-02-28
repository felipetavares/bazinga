#include "editor.h"

#include "gui.h"
#include "gui/label.h"
#include "gui/entry.h"
#include "gui/button.h"
#include "gui/spacer.h"
#include "gui/bgraph.h"
#include "gui/scroll.h"

using namespace bazinga;

void editor::openPropertiesWindow () {
	auto window = new gui::Window("Propriedades", 400, video::windowHeight);

	auto container = new gui::Container(gui::Container::VERTICAL);
	auto buttonsContainer = new gui::Container(gui::Container::HORIZONTAL, false, true);
	auto viewContainer = new gui::Container(gui::Container::HORIZONTAL);
	auto propertiesContainer = new gui::Container (gui::Container::VERTICAL, true);

	container->add(viewContainer);
	container->add(buttonsContainer);

	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	buttonsContainer->add(new gui::Button("Nova"));
	buttonsContainer->add(new gui::Button("Aplicar"));
	buttonsContainer->add(new gui::Button("Finalizar"));
	buttonsContainer->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
	buttonsContainer->borderLeft = buttonsContainer->borderRight = 0;
	buttonsContainer->borderTop = buttonsContainer->borderBottom = 0;

	viewContainer->add(propertiesContainer);
	auto scrollBar = new gui::Scroll(gui::Container::VERTICAL);
	viewContainer->add(scrollBar);
	viewContainer->borderLeft = viewContainer->borderRight = 0;
	viewContainer->borderTop = viewContainer->borderBottom = 0;

	for (int i=0;i<10;i++) {
		auto info = new gui::Container(gui::Container::HORIZONTAL,false,true);
		info->add(new gui::Label("propriedade"));
		info->add(new gui::Entry());
		info->add(new gui::Button("X"));

		propertiesContainer->add(info);
	}

	propertiesContainer->borderLeft = propertiesContainer->borderRight = 0;
	propertiesContainer->borderTop = propertiesContainer->borderBottom = 0;

	window->setRoot(container);
	gui::add(window);

	window->onUpdate = [=] (gui::Window* win) {
	};

	scrollBar->onChange = [=] (gui::Widget* wid) {
		propertiesContainer->scrollV(scrollBar->getScroll());
	};

	/*
	button->onClick = [=] (gui::Widget* wid) {
	  bazinga::quit();
	};
	*/
}