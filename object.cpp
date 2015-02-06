#include "bazinga.h"
#include "object.h"
#include "cache.h"
#include "filesystem.h"
#include "input.h"
#include "image.h"
#include <exception>
#include <SDL_opengl.h>
using namespace bazinga;

// The Lua API
void LAPI_log (string fname) {
  cout << "bazinga: Lua API: " << fname << "() " << "C" << endl;
}

int LAPI_new_object (lua_State* L) {
  LAPI_log ("LAPI_new_object");

  bazinga::getActiveMap()->newObject(L);

  return 0;
}

int LAPI_del_object (lua_State* L) {
  LAPI_log ("LAPI_del_object");

  if (lua_isnumber(L, 1)) {
    bazinga::getActiveMap()->deleteObject(lua_tonumber(L, 1));
  } else {

  }

  return 0;
}

int LAPI_new_font (lua_State* L) {
  LAPI_log ("LAPI_new_font");

  if (lua_isstring(L, 1) && lua_isstring(L, 2)) {
    string fontName = string(lua_tostring(L, 1));
    string internalName = string(lua_tostring(L, 2));
    cache::createFont(Path(fontName), internalName);
  } else {

  }  

  return 0;
}

int LAPI_set_font (lua_State* L) {
  LAPI_log("LAPI_set_font");

  if (lua_isstring(L, 1)) {
    string internalName = string(lua_tostring(L, 1));
    text::setFont(cache::getFont(internalName));
  }

  return 0;
}

int LAPI_set_align (lua_State* L) {
  LAPI_log("LAPI_set_align");

  if (lua_isstring(L, 1)) {
    string alignment = string(lua_tostring(L, 1));
    text::Align align;

    if (alignment == "center") {
      align = text::Center;
    } else if (alignment == "left") {
      align = text::Left;
    } else if (alignment == "right") {
      align = text::Right;
    } else {
      // Error, not a valid value
    }

    text::setAlign(align);
  }

  return 0;
}

int LAPI_fill_text (lua_State* L) {
  LAPI_log("LAPI_fill_text");

  if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
    auto txt = lua_tostring(L, 1);
    text::fillText(txt, lua_tonumber(L, 2), lua_tonumber(L, 3));
  }

  return 0;
}

int LAPI_set_camera (lua_State* L) {
  bazinga::getActiveMap()->setCamera(lua_tonumber(L, 1), lua_tonumber(L, 2));
}

int LAPI_new_dialog (lua_State* L) {
  LAPI_log ("LAPI_new_dialog");

  lua_pushnumber(L, bazinga::getActiveMap()->newDialog(lua_tostring(L, 1)));

  return 1;
}

int LAPI_del_dialog (lua_State* L) {
  LAPI_log ("LAPI_del_dialog");

  bazinga::getActiveMap()->deleteDialog(lua_tonumber(L, 1));

  return 0;
}

int LAPI_ended_dialog (lua_State* L) {
  // LAPI_log ("LAPI_ended_dialog");

  lua_pushboolean(L, bazinga::getActiveMap()->isDialogEnded(lua_tonumber(L, 1)));

  return 1;
}

int LAPI_search_object (lua_State* L) {
  LAPI_log ("LAPI_search_object");

  lua_pushnumber(L, bazinga::getActiveMap()->searchObject(string(lua_tostring(L, 1))));

  return 1;
}

int LAPI_hide_object (lua_State* L) {
  // LAPI_log ("LAPI_hide_object");

  bazinga::getActiveMap()->hideObject(lua_tonumber(L, 1), (bool)lua_toboolean(L, 2));

  return 1;
}

Object::Object (BjObject* jObject, int layer) {
  BjValue* jProperties = jObject->get("properties");

  L = NULL;

  if (jProperties->type == BjValue::jObject) {
    BjObject *properties = jProperties->object;
    int i;

    for (i=0;i<properties->keys.size();i++) {
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
  if (L != NULL) {
    lua_close(L);
  }

  cpSpace *pSpace = bazinga::getActiveMap()->getSpace();

  if (pShape != NULL) {
    if (pSpace != NULL)
      cpSpaceRemoveShape(pSpace, pShape);
    cpShapeFree(pShape);
  }

  if (pBody != NULL) {
    if (pSpace != NULL)
      cpSpaceRemoveBody(pSpace, pBody);
    cpBodyFree(pBody);
  }

  if (anim != NULL) {
    delete anim;
  }
}

void Object::loadFile (Path path) {
  script = path;

  // Creates a new Lua state
  L = luaL_newstate();
  luaL_openlibs (L);
  createLuaAPI(L);

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

void Object::init () {
  if (str_properties["script"] != "") {
    Path path = Path(str_properties["script"]);

    cout << "bazinga: script: loading from " << path.getPath() << endl;

    loadFile (path);
  }

  if (str_properties["anim"] != "") {
    anim = new Animation(Path(str_properties["anim"]));
  }

  if (num_properties["col"]) {
    cpSpace *pSpace = bazinga::getActiveMap()->getSpace();

    float x = num_properties["x"];
    float y = num_properties["y"];
    float w = num_properties["w"];
    float h = num_properties["h"];

    if (L) {
      cpVect verts[] = {
        cpv(0,0),
        cpv(0,h),
        cpv(w,h),
        cpv(w,0),
      };

      pBody = cpSpaceAddBody(pSpace, cpBodyNew(100, INFINITY));
      pShape = cpSpaceAddShape(pSpace, cpPolyShapeNew(pBody, 4, verts, cpvzero));
      cpBodySetPos (pBody, cpv(x,y));
      cpShapeSetElasticity(pShape, 0.0);
      cpShapeSetFriction(pShape, 0.0);
      cpShapeSetUserData(pShape, this);
    } else {
      cpVect verts[] = {
        cpv(x,y),
        cpv(x,y+h),
        cpv(x+w,y+h),
        cpv(x+w,y),
      };

      pShape = cpSpaceAddShape(pSpace, cpPolyShapeNew(pSpace->staticBody, 4, verts, cpvzero));
      cpShapeSetElasticity(pShape, 0.0);
      cpShapeSetFriction(pShape, 0.0);

      cpShapeSetUserData(pShape, this);
    }
  }

  num_properties["id"] = bazinga::getActiveMap()->getNewID();
}

void Object::update () {
  // If we have an script attached
  if (L) {
    if (num_properties["col"]) {
      // Get position from physical body and...
      cpVect position = cpBodyGetPos(pBody);

      // apply to lua props
      num_properties["x"] = position.x;
      num_properties["y"] = position.y;
    }

    input::setContextsIn(L);

    lua_getglobal(L, "update");
    createLuaProperties(L);

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

    if (num_properties["col"]) {
      cpBodySetPos(pBody, cpv(num_properties["x"], num_properties["y"]));
      cpBodyApplyImpulse(pBody, cpv(num_properties["vx"], num_properties["vy"]), cpBodyLocal2World(pBody, cpv(0,0)));
    }
  }
}

void Object::render () {
    if (str_properties["anim"] != "") {
      glPushMatrix();
          glTranslatef (num_properties["gx"]+num_properties["x"]+anim->getWidth()/2, num_properties["gy"]+num_properties["y"]+anim->getHeight()/2, 0);
          glRotatef(num_properties["ang"], 0, 0, 1);
          anim->render();
      glPopMatrix();
    } else if (str_properties["img"] != "") {
      Image* img = cache::getTexture(Path(str_properties["img"]));
      GLuint texID = img->id;

      float sX = float(img->w)/((float)img->rw);
      float sY = float(img->h)/((float)img->rh);

      glPushMatrix();
          glTranslatef (num_properties["gx"]+num_properties["x"]+img->w/2, num_properties["gy"]+num_properties["y"]+img->h/2, 0);
          glRotatef(num_properties["ang"], 0, 0, 1);
          glEnable (GL_TEXTURE);
          glEnable (GL_TEXTURE_2D);
          glBindTexture (GL_TEXTURE_2D, texID);
          glBegin(GL_QUADS);
            glTexCoord2f (0,1);       glVertex3f(-int(img->w)/2, -int(img->h)/2, 0);
            glTexCoord2f (sX,1);      glVertex3f(img->w/2, -int(img->h)/2, 0);
            glTexCoord2f (sX,1-sY);   glVertex3f(img->w/2, img->h/2, 0);
            glTexCoord2f (0,1-sY);    glVertex3f(-int(img->w)/2, img->h/2, 0);
          glEnd();
      glPopMatrix();
    }
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

void Object::createLuaProperties(lua_State* L) {
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

void Object::createLuaAPI (lua_State* L) {
  lua_newtable(L);

  // Create object
  // new_object(str)
  lua_pushstring(L, "new_object");
  lua_pushcfunction(L, LAPI_new_object);
  lua_settable(L, -3);

  // Delete object
  // del_object(num)
  // ONLY WORKS IN CALLBACKS!
  lua_pushstring(L, "del_object");
  lua_pushcfunction(L, LAPI_del_object);
  lua_settable(L, -3);

  // Move the camera to a fixed position
  // set_camera(num, num)
  lua_pushstring(L, "set_camera");
  lua_pushcfunction(L, LAPI_set_camera);
  lua_settable(L, -3);

  // Creates a new dialog
  // id = new_dialog(txt)
  lua_pushstring(L, "new_dialog");
  lua_pushcfunction(L, LAPI_new_dialog);
  lua_settable(L, -3);

  // Delete a dialog
  // del_dialog(id)
  lua_pushstring(L, "del_dialog");
  lua_pushcfunction(L, LAPI_del_dialog);
  lua_settable(L, -3);

  // Verify if a dialog has finished presenting
  // ended = ended_dialog(id)
  lua_pushstring(L, "ended_dialog");
  lua_pushcfunction(L, LAPI_ended_dialog);
  lua_settable(L, -3);

  // Create a font
  // new_font(oName, iName)
  lua_pushstring(L, "new_font");
  lua_pushcfunction(L, LAPI_new_font);
  lua_settable(L, -3);

  // Use a font
  // set_font(iName)
  lua_pushstring(L, "set_font");
  lua_pushcfunction(L, LAPI_set_font);
  lua_settable(L, -3);

  // Set alignment
  // set_align(align)
  lua_pushstring(L, "set_align");
  lua_pushcfunction(L, LAPI_set_align);
  lua_settable(L, -3);

  // Fill text
  // fill_text(txt, x, y)
  lua_pushstring(L, "fill_text");
  lua_pushcfunction(L, LAPI_fill_text);
  lua_settable(L, -3);

  // Globals for screen size
  // w
  lua_pushstring(L, "screen_w");
  lua_pushnumber(L, video::windowWidth);
  lua_settable(L, -3);

  // h
  lua_pushstring(L, "screen_h");
  lua_pushnumber(L, video::windowHeight);
  lua_settable(L, -3);

  // Search object
  // id = search_object(name)
  lua_pushstring(L, "search_object");
  lua_pushcfunction(L, LAPI_search_object);
  lua_settable(L, -3);

  // Hide object
  // hide_object(id, hide)
  lua_pushstring(L, "hide_object");
  lua_pushcfunction(L, LAPI_hide_object);
  lua_settable(L, -3);

  lua_setglobal(L, "bazinga");
}

bool bazinga::compareObjects (Object *obj, Object *obj2) {
    return ((obj->num_properties["y"]+obj->num_properties["h"]-obj->num_properties["z"]) <
           (obj2->num_properties["y"]+obj2->num_properties["h"]-obj2->num_properties["z"]));
}
