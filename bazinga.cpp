#include "bazinga.h"

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

namespace bazinga {
  double delta = 0;

  Map* themap = NULL;

  Map* getActiveMap () {
    return themap;
  }

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
        if (event.key.keysym.sym != SDLK_ESCAPE) {
          string keyName = string(SDL_GetKeyName(event.key.keysym.sym));
          transform (keyName.begin(), keyName.end(), keyName.begin(), ::tolower);
          input::keypress(keyName);
        } else
          return false;
        break;
        case SDL_KEYUP:
        if (event.key.keysym.sym != SDLK_ESCAPE) {
          string keyName = string(SDL_GetKeyName(event.key.keysym.sym));
          transform (keyName.begin(), keyName.end(), keyName.begin(), ::tolower);
          input::keyunpress(keyName);
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

    char *data = fs::getFileData(Path("maps/map1"));
    string sData = string (data);

    BjObject *object = json::parse (sData);

    themap = new Map();
    themap->init(object);
    
    /*
    delete object;
    delete data;
    */
  }

  void gameLoop () {
    cout << "gameLoop()" << endl;

    chrono::high_resolution_clock::time_point t;

    while (events()) {
      auto t0 = chrono::high_resolution_clock::now();
      auto elapsed = t0-t;
      t = t0;

      delta = ((std::chrono::duration_cast<std::chrono::microseconds>(elapsed)).count()/1000000.0);

      stringstream ss;
      ss << 1/delta;

      bazinga::video::setWindowTitleAndIcon(ss.str(),string(""));

      if (themap) {
        themap->update();
        video::renderMap(themap);
        video::render();
      }
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
  try {
    bazinga::video::init();
    bazinga::input::init();
    bazinga::audio::init();

    new bazinga::input::Context("main");
    bazinga::input::activateContext("main");

    bazinga::video::setWindowTitleAndIcon("Bazinga! Engine", "Bazinga! Engine");

    bazinga::startModules();
    bazinga::gameLoop();
    bazinga::endModules();

    bazinga::audio::deinit();
    bazinga::input::deinit();
    bazinga::video::deinit();
  } catch (exception e) {
    cout << "bazinga: exception: " << e.what() << endl;
  }

  return 0;
}
