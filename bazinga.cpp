#include "gl/glwin.h"
#include "bazinga.h"
#include "filesystem.h"
#include "text.h"
#include "cache.h"
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

namespace bazinga {
  lua_State *lCallbacks;
  double delta = 0;
  double curtime = 0;
  
  Map* activeMap = NULL;

  Map* getActiveMap () {
    return activeMap;
  }

  void postStepAct(cpSpace *space, cpArbiter *arb, void *data) {
    cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);
  }

  cpBool pBeginCollision (cpArbiter *arb, cpSpace *space, void *data) {
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)postStepAct, arb, NULL);

    cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);

    lua_getglobal(lCallbacks, "collision_begin");

    ((Object*)cpShapeGetUserData(a))->createLuaProperties(lCallbacks);
    ((Object*)cpShapeGetUserData(b))->createLuaProperties(lCallbacks);

    if (lua_pcall(lCallbacks, 2, 1, 0)) {
      cout << "bazinga: error when calling collision_begin() in callbacks.lua" << endl;

      if (lua_isstring(lCallbacks, -1)) {
        cout << "\t" << lua_tostring(lCallbacks, -1) << endl;
      }
    } else {
      if (lua_gettop(lCallbacks) < 1) { // This condition is not working ok
        cout << "bazinga: collision_begin() in callbacks.lua doesn't return value" << endl;
      } else {
        return lua_toboolean(lCallbacks, -1);
      }
    }

    // Default: collision will occur
    return true;
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

  bool init () {
    bazinga::video::init();
    bazinga::cache::init();
    bazinga::text::init();
    bazinga::input::init();
    bazinga::audio::init();

    // On Linux: nothing
    // On Windows: setup OpenGL above 1.1
    initGL();

    new bazinga::input::Context("main");
    bazinga::input::activateContext("main");

    bazinga::video::setWindowTitleAndIcon("Bazinga! Engine", "Bazinga! Engine");

    auto v = bazinga::cache::getVertexShader(bazinga::Path("shaders/default-vert.glsl"));
    auto f = bazinga::cache::getFragmentShader(bazinga::Path("shaders/default-frag.glsl"));
    bazinga::cache::createShaderProgram (v, f, "default");
    bazinga::cache::getShaderProgram("default")->loadUniforms({"sampler","color"});

    lCallbacks = luaL_newstate();
    luaL_openlibs (lCallbacks);
    Object::createLuaAPI(lCallbacks);

    // Loads the file
    if (luaL_dofile(lCallbacks, (Path("callbacks.lua")).getPath().c_str())) {
      cout << "bazinga: script contains errors" <<  endl;

      if (lua_isstring(lCallbacks, -1)) {
        cout << "\t" << lua_tostring(lCallbacks, -1) << endl;
      }

      lua_close(lCallbacks);
      lCallbacks = NULL;
    }

    auto mainScene = Path("scenes/main.scene");

    if (fs::fileExists(mainScene)) {
      char *data = fs::getFileData(mainScene);
      string sData = string (data);

      BjObject *object = json::parse (sData);

      activeMap = new Map();
      activeMap->init(object);
      
      delete object;
      delete data;
    } else {
      cout << "bazinga: scene: could not load " << mainScene.getPath() << endl;
      cout << "\t" << "aborting" << endl;
      return false;
    }

    return true;
  }

  void gameLoop () {
    //cout << "gameLoop()" << endl;

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
        video::render();
      }
    }
  }

  void deinit () {
    if (activeMap) {
      delete activeMap;
    }

    bazinga::audio::deinit();
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

  /*
  text::setAlign(text::Left);
  text::setBaseline(text::Alphabetic);

  font.setSize(32);
  text::fillText(ss.str(), 0, 0);

  font.setSize(12);
  text::fillText(ss.str(), 0, 32);

  text::setAlign(text::Right);
  font.setSize(32);
  font.setColor(0,1,0,1);
  text::fillText(ss.str(), 0, 64);

  text::setAlign(text::Center);
  font.setSize(32);
  font.setColor(1,1,1,1);
  text::fillText(ss.str(), 0, 128);
  */

  try {
    if (bazinga::init())
      bazinga::gameLoop();
    bazinga::deinit();
  } catch (exception e) {
    cout << "bazinga: exception: " << e.what() << endl;
  }

  return 0;
}
