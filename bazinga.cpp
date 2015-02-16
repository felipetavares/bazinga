#include "gl/glwin.h"
#include "bazinga.h"
#include "filesystem.h"
#include "text.h"
#include "cache.h"
#include "gui.h"
#include "gui/label.h"
#include "gui/button.h"
#include "gui/spacer.h"
#include "gui/slider.h"
#include "gui/progress.h"
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

namespace bazinga {
  double delta = 0;
  double curtime = 0;
  bool console = false;
  bool setNewScene = false;
  Path newScenePath;
  string mainScenePath = "scenes/main.scene";

  Map* activeMap = NULL;

  Map* getActiveMap () {
    return activeMap;
  }

  //void postStepAct(cpSpace *space, cpArbiter *arb, void *data) {
  //  cpShape *a, *b;
  //  cpArbiterGetShapes(arb, &a, &b);
  //}

  cpBool pBeginCollision (cpArbiter *arb, cpSpace *space, void *data) {
    return getActiveMap()->pmBeginCollision(arb, space, data);
  }

  void toggleConsole() {
    console = !console;
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
          input::mousemove (event.motion.x-video::windowWidth/2,
                            event.motion.y-video::windowHeight/2);
        break;
        case SDL_MOUSEBUTTONDOWN:
          input::mousepress (event.button.button,
                             event.button.x-video::windowWidth/2,
                             event.button.y-video::windowHeight/2);
        break;
        case SDL_MOUSEBUTTONUP:
          input::mouseunpress (event.button.button,
                               event.button.x-video::windowWidth/2,
                               event.button.y-video::windowHeight/2);
        break;
        case SDL_QUIT:
          return false;
      }
    }

    return true;
  }

  void setScene (Path path) {
    bazinga::newScenePath = path;
    bazinga::setNewScene = true;
  }

  bool init () {
    bazinga::video::init();
    bazinga::cache::init();
    bazinga::text::init();
    bazinga::input::init();
    //bazinga::audio::init();
    bazinga::gui::init();

    // On Linux: nothing
    // On Windows: setup OpenGL above 1.1
    initGL();

    new bazinga::input::Context("main");
    bazinga::input::activateContext("main");

    bazinga::video::setWindowTitleAndIcon("Bazinga! Engine", "Bazinga! Engine");
    bazinga::video::setWindowIcon(Path("assets/images/icon.bmp"));

    auto v = bazinga::cache::getVertexShader(bazinga::Path("shaders/default-vert.glsl"));
    auto f = bazinga::cache::getFragmentShader(bazinga::Path("shaders/default-frag.glsl"));
    bazinga::cache::createShaderProgram (v, f, "default");
    bazinga::cache::getShaderProgram("default")->loadUniforms({"sampler","color"});

    setScene(Path(mainScenePath));

    return true;
  }

  void gameLoop () {
    //cout << "gameLoop()" << endl;
    cache::createFont(Path("assets/fonts/texgyrecursor-bold.otf"), "default");

    auto window = new gui::Window("About", 800, 600);
    auto container = new gui::Container(gui::Container::VERTICAL);
    auto line = new gui::Container(gui::Container::HORIZONTAL);
    auto line2 = new gui::Container(gui::Container::HORIZONTAL);
    auto line3 = new gui::Container(gui::Container::HORIZONTAL);

    line->borderLeft = line->borderRight = 0;
    line->borderTop = line->borderBottom = 0;

    line2->borderLeft = line2->borderRight = 0;
    line2->borderTop = line2->borderBottom = 0;

    line3->borderLeft = line3->borderRight = 0;
    line3->borderTop = line3->borderBottom = 0;

    line->add(new gui::Label("Set to 100%:"));
    auto button = new gui::Button("100%");
    line->add(button);

    line2->add(new gui::Label("Adjust:"));
    auto slider = new gui::Slider(0.5);
    line2->add(slider);

    line3->add(new gui::Label("This much:"));
    auto pbar = new gui::Progress(0.5);
    line3->add(pbar);

    container->add(line);
    container->add(line2);
    container->add(line3);

    container->add(new gui::Spacer(gui::Spacer::VERTICAL));

    window->setRoot(container);
    gui::add(window);

    slider->onChange = [=] (gui::Widget* wid) {
      pbar->setPosition(((gui::Slider*)wid)->getPosition());
    };

    button->onClick = [=] (gui::Widget* wid) {
      pbar->setPosition(1);
    };

    chrono::high_resolution_clock::time_point t;
    chrono::high_resolution_clock::time_point startTime;

    startTime = chrono::high_resolution_clock::now();

    while (events()) {
      auto t0 = chrono::high_resolution_clock::now();
      auto elapsed = t0-t;
      t = t0;

      curtime = (chrono::duration_cast<chrono::microseconds>((t0-startTime))).count()/1000000.0;
      delta = ((chrono::duration_cast<chrono::microseconds>(elapsed)).count()/1000000.0);

      if (delta > 1.0/30.0) {
        delta = 1.0/30.0;
      }

      /*
      stringstream ss;
      ss << 1/delta;

      bazinga::video::setWindowTitleAndIcon(ss.str(),string("12"));
      */

      if (activeMap) {
        activeMap->update();

        video::renderMap(activeMap);
     
        //if (console) {
        //  video::setColor(1,0,0,1);
        //  video::fillRect(-100,-100,200,200);
        //}

        gui::render();

        video::render();
      }

      if (bazinga::setNewScene) {
        if (activeMap) {
          delete activeMap;
        }

        setNewScene = false;

        activeMap = new Map(newScenePath);
        activeMap->init();
      }

    }
  }

  void deinit () {
    if (activeMap) {
      delete activeMap;
    }

    bazinga::gui::deinit();
    //bazinga::audio::deinit();
    bazinga::input::deinit();
    bazinga::text::deinit();
    bazinga::cache::deinit();
    bazinga::video::deinit();
  }
}

void copyMaps () {
#ifdef _WIN32
  // List map dir
  bool ok;
  auto path = bazinga::Path(string(getenv("APPDATA")+string("/LOVE/data/maps/")));
  auto dir = bazinga::fs::listDirectory(path, ok);

  if (ok) {
    cout << "bazinga: scene copy: copying the following scenes to scenes/" << endl;

    for (auto e :dir) {
      if (!bazinga::fs::isDir(e)) {
        cout << "\t - " << e.getPath() << " - ";
        if (bazinga::fs::copyFile(e, bazinga::Path("scenes/"+e.getName()), true)) {
          cout << "ok" << endl;
        } else {
          cout << "fail" << endl;
        }
      }
    }
  } else {
    cout << "bazinga: scene copy: can't find any scenes in: " << path.getPath() << endl;
  }
#endif
}

#ifdef _WIN32
int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
 #else
int main (int argc, char** argv) {
#endif /* _WIN32 */ 
  copyMaps();

  try {
    if (bazinga::init())
      bazinga::gameLoop();
    bazinga::deinit();
  } catch (exception e) {
    cout << "bazinga: exception: " << e.what() << endl;
  }

  return 0;
}
