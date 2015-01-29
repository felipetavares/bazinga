#ifndef BAZINGA_OBJECT_H
#define BAZINGA_OBJECT_H

#include <string>
#include <map>
#include "filesystem.h"
#include "json.h"
#include "animation.h"
#include <chipmunk.h>
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
    cpShape *pShape = NULL;
    cpBody *pBody = NULL;
    Animation *anim = NULL;
  public:
    map <string, float>   num_properties;
    map <string, string>  str_properties;

    Object();
    Object(Path);
    Object(BjObject*, int);
    ~Object();

    void init();
    void update();
    void render();

    bool operator< (const Object&);
    void createLuaProperties(lua_State*);
    static void createLuaAPI(lua_State*);
  private:
    void loadFile(Path);
    void updateProperties();
  };

  bool compareObjects(Object*, Object*);
}

#endif /* BAZINGA_OBJECT_H */
