#ifndef BAZINGA_OBJECT_H
#define BAZINGA_OBJECT_H

#include <string>
#include <map>
#include "filesystem.h"
#include "json.h"
using namespace std;

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

namespace bazinga {
  class Object {
    map <string, float>   num_properties;
    map <string, string>  str_properties;

    Path script;

    lua_State *L;
  public:
    Object();
    Object(Path);
    Object(BjObject*);
    ~Object();

    void update();

  private:
    void loadFile(Path);
    void updateProperties();
    void createLuaProperties();
  };
}

#endif /* BAZINGA_OBJECT_H */
