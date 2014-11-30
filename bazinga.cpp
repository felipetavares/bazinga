#include "bazinga.h"

#include <iostream>
using namespace std;

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
  #include <SDL.h>
}

namespace bazinga {
  void startModules () {
    cout << "startModules()" << endl;

    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs (L);

    if (luaL_dofile(L, "main.lua")) {
      cout << "bazinga: error when running file main.lua" << endl;
    }

    lua_close(L);

    char *data = fs::getFileData(Path("maps:test.json"));
    string sData = string (data);

    BjObject *object = json::parse (sData);

    delete object;

    delete data;
  }

  void gameLoop () {
    cout << "gameLoop()" << endl;
  }

  void endModules () {
    cout << "endModules()" << endl;
  }
}

int main (int argc, char** argv) {
  bazinga::startModules();
  bazinga::gameLoop();
  bazinga::endModules();
  return 0;
}
