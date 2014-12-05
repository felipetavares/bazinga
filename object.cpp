#include "object.h"
#include "cache.h"
#include "filesystem.h"
#include <exception>
#include <SDL_opengl.h>
using namespace bazinga;

Object::Object (BjObject* jObject) {
  BjValue* jProperties = jObject->get("properties");

  L = NULL;

  if (jProperties->type == BjValue::jObject) {
    BjObject *properties = jProperties->object;
    int i;

    for (i=0;i<properties->keys.size();i++) {
      if (properties->keys[i] == "script") {
        Path path = Path(properties->values[i]->str);

        loadFile (path);
      } else {
        BjValue *jOValue = properties->values[i];

        if (jOValue->type == BjValue::jNumber) {
          num_properties[properties->keys[i]] = jOValue->number;
        } else
        if (jOValue->type == BjValue::jString) {
          str_properties[properties->keys[i]] = jOValue->str;
        } else {
            // Error
        }
      }
    }
  } else {
    // Error
  }
}

Object::Object (Path path) {
  loadFile(path);
}

Object::Object () {
  // We don't have any lua script attached to this object
  L = NULL;
}

Object::~Object () {
  if (L) {
    lua_close(L);
  }
}

void Object::loadFile (Path path) {
  script = path;

  // Creates a new Lua state
  L = luaL_newstate();
  luaL_openlibs (L);

  // Loads the file
  if (luaL_dofile(L, script.getPath().c_str())) {
    cout << "bazinga: script contains errors" << endl;

    if (lua_isstring(L, -1)) {
      cout << "\t" << lua_tostring(L, -1) << endl;
    }

    lua_close(L);
    L = NULL;
  }
}

void Object::update () {
  // If we have an script attached
  if (L) {
    lua_getglobal(L, "update");

    createLuaProperties();

    if (lua_pcall(L, 1, 1, 0)) {
      cout << "bazinga: error when calling update() in script " << script.getPath() << endl;

      if (lua_isstring(L, -1)) {
        cout << "\t" << lua_tostring(L, -1) << endl;
      }
    } else {
      if (lua_gettop(L) < 1) { // This condition is not working ok
        cout << "bazinga: update() at script " << script.getPath() << " doesn't return self" << endl;
      } else
        updateProperties();
    }
  }
}

void Object::render () {
  float sX = 1;
  float sY = 1;
  glPushMatrix();
    glTranslatef (num_properties["x"]-200, num_properties["y"]-200, 0);
    glEnable (GL_TEXTURE);
    glEnable (GL_TEXTURE_2D);
    glDisable (GL_ALPHA_TEST);
    glDisable (GL_BLEND);
    GLuint texID = cache::getTexture(Path(str_properties["img"]));
    cout << "bazinga: texID: " << texID << endl;
    glBindTexture (GL_TEXTURE_2D, texID);
    glBegin(GL_QUADS);
    glTexCoord2f (0,0); 	glVertex3f(0, 0, 0);
    glTexCoord2f (sX,0); 	glVertex3f(num_properties["w"], 0, 0);
    glTexCoord2f (sX,sY); 	glVertex3f(num_properties["w"], num_properties["h"], 0);
    glTexCoord2f (0,sY); 	glVertex3f(0, num_properties["h"], 0);
    glEnd();
  glPopMatrix();
}

void Object::updateProperties() {
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);

    const char *key = lua_tostring(L, -1);

    if (lua_isnumber(L, -2)) {
      float value = lua_tonumber(L, -2);
      num_properties[string(key)] = value;
    } else {
      const char *value = lua_tostring(L, -2);
      str_properties[string(key)] = string(value);
    }

    lua_pop(L, 2);
  }
}

void Object::createLuaProperties() {
  lua_newtable(L);

  map<string, float>::iterator n;
  for (n=num_properties.begin();n!=num_properties.end();n++) {
    lua_pushstring(L, n->first.c_str());
    lua_pushnumber(L, n->second);
    lua_settable(L, -3);
  }

  map<string, string>::iterator s;
  for (s=str_properties.begin();s!=str_properties.end();s++) {
    lua_pushstring(L, s->first.c_str());
    lua_pushstring(L, s->second.c_str());
    lua_settable(L, -3);
  }
}
