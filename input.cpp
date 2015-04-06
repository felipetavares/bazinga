#include "input.h"
#include "bazinga.h"
#include "gui.h"
#include "map.h"
#include "console.h"
#include <iostream>
#include <algorithm>
using namespace bazinga;

lua_State *input::L = NULL;
map <string, input::Context*> input::contexts;
map <string, bool> input::keyboard;
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
      console <<  "bazinga: error when calling " + (name+"_keypress") + " in mapper.lua for context " + name  << outline;

      if (lua_isstring(L, -1)) {
        console <<  "\t" + string(lua_tostring(L, -1) ) << outline;
      }
    } else {
      if (lua_gettop(L) < 1) { // This condition is not working ok
        console <<  "bazinga: " + (name+"_keypress") + "() in mapper.lua doesn't return self!"  << outline;
      } else {
        updateArgs();
      }
    }
  }

  return true;
}

bool input::Context::keyunpress (string key) {
  // If we have an script attached
  if (L) {
    lua_getglobal(L, (name+"_keyunpress").c_str());

    createArgs();

    lua_pushstring (L, key.c_str());

    // Add key as argument
    if (lua_pcall(L, 2, 1, 0)) {
      console <<  "bazinga: error when calling " + (name+"_keyunpress") + " in mapper.lua for context " + name  << outline;

      if (lua_isstring(L, -1)) {
        console <<  "\t" + string(lua_tostring(L, -1) ) << outline;
      }
    } else {
      if (lua_gettop(L) < 1) { // This condition is not working ok
        console <<  "bazinga: " + (name+"_keyunpress") + "() in mapper.lua doesn't return self!"  << outline;
      } else {
        updateArgs();
      }
    }
  }

  return true;
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
 if (!gui::mousemove(x, y)) {
  if (getActiveMap())
    getActiveMap()->mousemove(x, y);
 }
}

void input::mousepress (int button, int x, int y) {
  if (!gui::mousepress(button, x, y)) {
    if (getActiveMap())
      getActiveMap()->mousepress(button, x, y);
  }
}

void input::mouseunpress (int button, int x, int y) {
  if (!gui::mouseunpress(button, x, y)) {
    if (getActiveMap())
      getActiveMap()->mouseunpress(button, x, y);
  }
}

void input::keypress(string key, uint16_t unicode) {
  if (key == "tab") {
    toggleConsole();
    return;
  }

  if (key == "page up") {
    console.up();
    return;
  }

  if (key == "page down") {
    console.down();
    return;
  }

  console << LINEINFO << "" << key  << outline;

  keyboard[key] = true;

  if (gui::keypress(unicode, key))
    return;

  for (auto c :active) {
    c->keypress(key);
  }
}

void input::keyunpress(string key) {
  if (key == "tab" || key == "page up" || key == "page down") {
    return;
  }

  console << LINEINFO << "" << key  << outline;

  keyboard[key] = false;

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
  console << LINEINFO << outline;

  L = luaL_newstate();
  luaL_openlibs (L);

  // Loads the file
  if (luaL_dofile(L, "mapper.lua")) {
    console <<  "bazinga: mapper.lua contains errors"  << outline;

    if (lua_isstring(L, -1)) {
      console <<  "\t" + string(lua_tostring(L, -1) ) << outline;
    }

    lua_close(L);
    L = NULL;
  }
}

void input::deinit () {
  console << LINEINFO << outline;

  if (L) {
    lua_close(L);
  }

  for (auto i=contexts.begin();i!=contexts.end();i++) {
    delete i->second;
  }
}
