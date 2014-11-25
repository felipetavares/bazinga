#include "bazinga.h"

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

namespace bazinga {
  void startModules () {
    lua_State* L = luaL_newstate();
  }

  void gameLoop () {

  }

  void endModules () {

  }
}

int main (int argc, char** argv) {
  bazinga::startModules();
  bazinga::gameLoop();
  bazinga::endModules();
  return 0;
}
