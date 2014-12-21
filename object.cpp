#include "bazinga.h"
#include "object.h"
#include "cache.h"
#include "filesystem.h"
#include "input.h"
#include <exception>
#include <SDL_opengl.h>
using namespace bazinga;

Object::Object (BjObject* jObject, int layer) {
  BjValue* jProperties = jObject->get("properties");

  L = NULL;

  if (jProperties->type == BjValue::jObject) {
    BjObject *properties = jProperties->object;
    int i;

    for (i=0;i<properties->keys.size();i++) {
      if (properties->keys[i] == "script") {
        Path path = Path(properties->values[i]->str);

        cout << "bazinga: script: loading from " << path.getPath() << endl;

        loadFile (path);
      } else {
        BjValue *jOValue = properties->values[i];

        if (jOValue->type == BjValue::jNumber) {
          if (properties->keys[i] == "z") {
            num_properties[properties->keys[i]] = jOValue->number-(layer*100);
          } else {
            num_properties[properties->keys[i]] = jOValue->number;
          }
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

  cpSpace *pSpace = bazinga::getActiveMap()->getSpace();

  float x = num_properties["x"];
  float y = num_properties["y"];
  float w = num_properties["w"];
  float h = num_properties["h"];

  cpVect verts[] = {
    cpv(x-w/2,y-h/2),
    cpv(x-w/2,y+h/2),
    cpv(x+w/2,y+h/2),
    cpv(x+w/2,y-h/2),
  };

  cpPolyShapeNew(pSpace->staticBody, 4, verts, cpvzero);
}

Object::Object (Path path) {
  loadFile(path);
}

Object::Object () {
  // We don't have any lua script attached to this object
  L = NULL;
}

Object::~Object () {
  if (L != NULL) {
    lua_close(L);
  }

  if (pShape != NULL) {
    cpShapeFree(pShape);
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
    input::setContextsIn(L);

    lua_getglobal(L, "update");
    createLuaProperties();

    lua_pushnumber(L, bazinga::delta);
    lua_setglobal(L, "dt");

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
    video::Image img = cache::getTexture(Path(str_properties["img"]));
    GLuint texID = img.id;

    float sX = float(img.w)/((float)img.rw);
    float sY = float(img.h)/((float)img.rh);

    glPushMatrix();
        glTranslatef (num_properties["x"]-400, num_properties["y"]-200, 0);
        glEnable (GL_TEXTURE);
        glEnable (GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, texID);
        glBegin(GL_QUADS);
          glTexCoord2f (0,1);  glVertex3f(0, 0, 0);
          glTexCoord2f (sX,1); 	  glVertex3f(img.w, 0, 0);
          glTexCoord2f (sX,1-sY);   glVertex3f(img.w, img.h, 0);
          glTexCoord2f (0,1-sY); 	  glVertex3f(0, img.h, 0);
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

bool bazinga::compareObjects (Object *obj, Object *obj2) {
    return ((obj->num_properties["y"]+obj->num_properties["h"]-obj->num_properties["z"]) <
           (obj2->num_properties["y"]+obj2->num_properties["h"]-obj2->num_properties["z"]));
}
