#include "bazinga.h"

#include <iostream>
#include <vector>
using namespace std;

namespace bazinga {
  Map* themap = NULL;

  bool events () {
    SDL_Event event;

    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
          //inputManager->joystickButtonEvent (event);
        break;
        case SDL_JOYAXISMOTION:
          //inputManager->joystickAxisEvent (event);
        break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        if (event.key.keysym.sym != SDLK_ESCAPE) {
          //inputManager->keyboardKeyEvent (event);
        } else
          return false;
        break;
        case SDL_MOUSEMOTION:
          //inputManager->mouseMoveEvent (event);
        break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          //inputManager->mouseButtonEvent (event);
        break;
        case SDL_QUIT:
          return false;
      }
    }

    return true;
  }

  void startModules () {
    cout << "startModules()" << endl;

    char *data = fs::getFileData(Path("C:/Users/Rachel/AppData/Roaming/LOVE/data/maps/sala 1"));
    string sData = string (data);

    BjObject *object = json::parse (sData);

    themap = new Map(object);

    delete object;
    delete data;
  }

  void gameLoop () {
    cout << "gameLoop()" << endl;

    while (events()) {
      themap->update();
      video::renderMap(themap);
      video::render();
    }
  }

  void endModules () {
    cout << "endModules()" << endl;

    if (themap) {
      delete themap;
    }
  }
}

int main (int argc, char** argv) {
  bazinga::video::init();

  bazinga::video::setWindowTitleAndIcon("Bazinga! Engine", "Bazinga! Engine");

  bazinga::startModules();
  bazinga::gameLoop();
  bazinga::endModules();

  bazinga::video::deinit();
  return 0;
}
