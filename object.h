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
    Path script;
    lua_State *L;
  public:
    map <string, float>   num_properties;
    map <string, string>  str_properties;

    Object();
    Object(Path);
    Object(BjObject*);
    ~Object();

    void update();
    void render();

    bool operator< (const Object&);
  private:
    void loadFile(Path);
    void updateProperties();
    void createLuaProperties();
  };

  bool compareObjects(Object*, Object*);
}

#endif /* BAZINGA_OBJECT_H */
