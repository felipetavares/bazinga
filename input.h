#ifndef BAZINGA_INPUT_H
#define BAZINGA_INPUT_H

#include <map>
#include <vector>
#include <string>
using namespace std;

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>
}

namespace bazinga {
  namespace input {
    extern lua_State *L;

    class Context {
      map <string, float> ctx; // Passed to/from Lua scripts
    public:
      string name;

      // Name
      Context (string);

      bool keypress (string);
      bool keyunpress (string);
      void createArgs(lua_State *L=input::L);
      void updateArgs();
    };

    extern map <string, Context*> contexts;
    extern vector <Context*> active;
    extern map <string, bool> keyboard;

    void addContext (string, Context*);
    void activateContext (string);

    void keypress(string, uint16_t);
    void keyunpress(string);
    void mousemove(int, int);
    void mousepress(int, int, int);
    void mouseunpress(int, int, int);

    void setContextsIn (lua_State*);

    void init();
    void deinit();
  }
}

#endif /* BAZINGA_INPUT_H */
