#include "input.h"
#include "bazinga.h"
#include "gui.h"
#include <iostream>
#include <algorithm>
using namespace bazinga;

lua_State *input::L = NULL;
map <string, input::Context*> input::contexts;
vector <input::Context*> input::active;

input::Context::Context (string name) {
  this->name = name;
  input::addContext(name, this);
}

bool input::Context::keypress (string key) {
  // If we have an script attached
  if (L) {
    lua_getglobal(L, (name+"_keypress").c_str());

    createArgs();

    lua_pushstring (L, key.c_str());

    // Add key as argument
    if (lua_pcall(L, 2, 1, 0)) {
      cout << "bazinga: error when calling " << (name+"_keypress") <<" in mapper.lua for context " << name << endl;

      if (lua_isstring(L, -1)) {
        cout << "\t" << lua_tostring(L, -1) << endl;
      }
    } else {
      if (lua_gettop(L) < 1) { // This condition is not working ok
        cout << "bazinga: " << (name+"_keypress") << "() in mapper.lua doesn't return self!" << endl;
      } else {
        updateArgs();
      }
    }
  }
}

bool input::Context::keyunpress (string key) {
  // If we have an script attached
  if (L) {
    lua_getglobal(L, (name+"_keyunpress").c_str());

    createArgs();

    lua_pushstring (L, key.c_str());

    // Add key as argument
    if (lua_pcall(L, 2, 1, 0)) {
      cout << "bazinga: error when calling " << (name+"_keyunpress") <<" in mapper.lua for context " << name << endl;

      if (lua_isstring(L, -1)) {
        cout << "\t" << lua_tostring(L, -1) << endl;
      }
    } else {
      if (lua_gettop(L) < 1) { // This condition is not working ok
        cout << "bazinga: " << (name+"_keyunpress") << "() in mapper.lua doesn't return self!" << endl;
      } else {
        updateArgs();
      }
    }
  }
}
void input::Context::createArgs (lua_State *L) {
  // Push the table
  lua_newtable(L);

  map<string, float>::iterator n;
  for (n=ctx.begin();n!=ctx.end();n++) {
    lua_pushstring(L, n->first.c_str());
    lua_pushnumber(L, n->second);
    lua_settable(L, -3);
  }
}

void input::Context::updateArgs () {
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);

    const char *key = lua_tostring(L, -1);

    float value = lua_tonumber(L, -2);
    ctx[string(key)] = value;

    lua_pop(L, 2);
  }
}

void input::addContext (string name, Context* ctx) {
  contexts[name] = ctx;
}

void input::activateContext (string name) {
  Context *ctx = contexts[name];
  active.push_back(ctx);
}

void input::mousemove (int x, int y) {
  gui::mousemove(x, y);
}

void input::mousepress (int button, int x, int y) {
  gui::mousepress(button, x, y);
}

void input::mouseunpress (int button) {
  gui::mouseunpress(button);
}

void input::keypress(string key) {
  cout << "bazinga: input: keydown: " << key << endl;

  if (key == "tab") {
    toggleConsole();
  }

  for (auto c :active) {
    c->keypress(key);
  }
}

void input::keyunpress(string key) {
  cout << "bazinga: input: keyup: " << key << endl;

  for (auto c :active) {
    c->keyunpress(key);
  }
}

void input::setContextsIn (lua_State *L) {
  for (auto c :active) {
    c->createArgs(L);
    lua_setglobal(L, ("ctx_"+c->name).c_str());
  }
}

void input::init () {
  cout << "bazinga: input: init()" << endl;

  L = luaL_newstate();
  luaL_openlibs (L);

  // Loads the file
  if (luaL_dofile(L, "mapper.lua")) {
    cout << "bazinga: mapper.lua contains errors" << endl;

    if (lua_isstring(L, -1)) {
      cout << "\t" << lua_tostring(L, -1) << endl;
    }

    lua_close(L);
    L = NULL;
  }
}

void input::deinit () {
  cout << "bazinga: input: deinit()" << endl;

  if (L) {
    lua_close(L);
  }

  for (auto i=contexts.begin();i!=contexts.end();i++) {
    delete i->second;
  }
}
