#include "bazinga.h"

#include <iostream>
#include <vector>
using namespace std;

namespace bazinga {
  void startModules () {
    cout << "startModules()" << endl;

    Object o = Object(Path("main.lua"));

    o.update();

    /*
    lua_getglobal(L, "update");

    lua_newtable(L);

    lua_pushstring(L, "x");
    lua_pushnumber(L, 150);
    lua_settable(L, -3);

    lua_pushstring(L, "y");
    lua_pushnumber(L, 150);
    lua_settable(L, -3);

    if (lua_pcall(L, 1, 1, 0)) {
      cout << "bazinga: error when calling update() in file main.lua" << endl;

      if (lua_isstring(L, -1)) {
        cout << "\t" << lua_tostring(L, -1) << endl;
      }
    } else {

    }

    lua_close(L);
    */

    /*
    char *data = fs::getFileData(Path("maps:test.json"));
    string sData = string (data);

    BjObject *object = json::parse (sData);

    delete object;
    delete data;
    */
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
