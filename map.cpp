#include "map.h"
#include "bazinga.h"
#include "text.h"
using namespace bazinga;

Dialog::Dialog (int id, string text):
  id(id), text(text), textPosition(0), bufferPosition(0), nextTime(0),
  ended(false) {
    sizeW = video::windowWidth/2;
    intervalTime = 0.05;
}

void Dialog::update () {
  if (textPosition < text.size()) {
    if (curtime > nextTime) {
      nextTime = curtime+intervalTime;
      fillChar();
    }
  } else {
    ended = true;
  }
}

void Dialog::render () {
  int startY = video::windowHeight/2-20*bufferSize;

  video::setColor(0.2, 0.2, 0.2);
  video::fillRect(-sizeW/2-10, startY-10, sizeW+20, 20*bufferSize+10);

  for (int i=0;i<bufferSize;i++)
    text::fillText(buffer[i], -sizeW/2, startY+i*20+10);
}

void Dialog::fillChar () {
  buffer[bufferPosition] += text[textPosition++];

  auto size = text::measureText(buffer[bufferPosition]);

  if (size.w > sizeW) {
    if (bufferPosition < bufferSize-1)
      bufferPosition++;
    else {
      for (int i=0;i<bufferSize-1;i++) {
        buffer[i] = buffer[i+1];
      }
      buffer[bufferSize-1] = "";
    }
  }
}

int Dialog::getID() {
  return id;
}

bool Dialog::isEnded () {
  return ended;
}

Layer::Layer (BjObject *jLayer, Map* map, int layer) {
  BjValue* jActive = jLayer->get("active");

  if (jActive->type == BjValue::jTrue) {
    //active = true;
  } else
  if (jActive->type == BjValue::jFalse) {
    //active = false;
  } else {
    // Error
  }

  BjValue* jData = jLayer->get("data");

  if (jData->type == BjValue::jArray) {
    BjArray *array = jData->array;
    int i;

    for (i=0;i<array->array.size();i++) {
      BjValue *jOValue = array->array[i];

      if (jOValue->type == BjValue::jObject) {
        auto o = new Object(jOValue->object, layer);
        o->init();
        map->addObject(o);
      } else {
        // Error
      }
    }
  } else {
    // Error
  }
}

Layer::~Layer () {
}

int Map::oid = 0;
int Map::did = 0;

Map::Map () {
  cout << "bazinga: chipmunk: initializing space...";

  pSpace = cpSpaceNew();
  cpSpaceSetDamping (pSpace, 0.01);

  if (pSpace) {
    cout << " ok" << endl;
  } else {
    cout << " fail" << endl;
  }

  cout << "bazinga: chipmunk: initializing collision handler..." << endl;

  cpSpaceSetDefaultCollisionHandler(pSpace, pBeginCollision, NULL, NULL, NULL, NULL);

  zoomX = 1;
  zoomY = 1;
}

int Map::getNewID() {
  return oid++;
}

void Map::init (BjObject *jMap) {
  // Get an array called "layers" and iterate over it
  BjValue* jLayers = jMap->get("layers");

  // Is it an array?
  if (jLayers->type == BjValue::jArray) {
    BjArray *array = jLayers->array;

    for (int i=0;i<array->array.size();i++) {
      BjValue *jOValue = array->array[i];

      if (jOValue->type == BjValue::jObject) {
        Layer(jOValue->object, this, i);
      } else {
        // Error
      }
    }
  } else {
    // Some kind of error
  }

  cout << "bazinga: scene: " << objects.size() << " objects loaded" << endl;
}

Map::~Map () {
  if (pSpace)
    cpSpaceFree(pSpace);
}

int Map::newDialog (string text) {
  int dialogID = did++;

  dialogs.push_back(new Dialog(dialogID, text));

  return dialogID;
}

void Map::deleteDialog (int id) {
  int i;
  for (int i=0;i<dialogs.size();i++) {
    auto d = dialogs[i];
    if (d->getID() == id) {
      dialogs.erase(dialogs.begin()+i);
      delete d;
      break;
    }
  }
}

bool Map::isDialogEnded (int id) {
  int i;
  for (int i=0;i<dialogs.size();i++) {
    auto d = dialogs[i];
    if (d->getID() == id) {
      return d->isEnded();
    }
  }
}

void Map::setCamera (float x, float y) {
  camx = x;
  camy = y;
}

void Map::setZoom (float x, float y) {
  zoomX = x;
  zoomY = y;
}

cpSpace* Map::getSpace () {
  return pSpace;
}

void Map::deleteObject (int id) {
  for (int i=0;i<objects.size();i++) {
    if (objects[i]->num_properties["id"] == id) {
      objects[i]->num_properties["delete"] = 1;
    }
  }    
}

int Map::searchObject (string name) {
  for (int i=0;i<objects.size();i++) {
    if (objects[i]->str_properties["name"] == name) {
      return objects[i]->num_properties["id"];
    }    
  }
}

void Map::hideObject (int id, bool hide) {
  for (int i=0;i<objects.size();i++) {
    if (objects[i]->num_properties["id"] == id) {
      objects[i]->num_properties["hidden"] = (int)hide;
    }    
  }
}

// Creates an object from an object description file
// Called from Lua
int Map::newObject (lua_State *L) {
  // Get file path
  Path fpath = Path(string(lua_tostring(L, 1)));

  char *data = fs::getFileData(fpath);
  string sData = string (data);

  BjObject *object = json::parse (sData);
  
  Object *o = new Object(object, 0);

  lua_insert(L, 2);

  // Properties to be changed
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);

    const char *key = lua_tostring(L, -1);

    if (lua_isnumber(L, -2)) {
      float value = lua_tonumber(L, -2);
      o->num_properties[string(key)] = value;
    } else {
      const char *value = lua_tostring(L, -2);
      o->str_properties[string(key)] = string(value);
    }

    lua_pop(L, 2);
  }

  o->init();
  addObject(o);

  delete object;
  delete data;

  // Number of results
  return 0;
}

void Map::setGravity (float x, float y) {
  pSpace->gravity = cpv(x, y);
}

void Map::setReorder (bool reorder) {
  this->reorder = reorder;
}

void Map::addObject (Object *object) {
  objects.push_back(object);
}

void Map::update() {
  for (int i=0;i<objects.size();i++) {
    if (objects[i]->num_properties["delete"]) {
      delete objects[i];
      objects.erase(objects.begin()+i);
      i--;
    } else
      objects[i]->update();
  }

  for (int i=0;i<dialogs.size();i++) {
    dialogs[i]->update();
  }

  cpSpaceStep(pSpace, bazinga::delta);
}

void Map::render() {
  glPushMatrix();
  glScalef(zoomX, zoomY, 0);

  glTranslatef (-camx, -camy, 0);

  if (reorder)
    sort (objects.begin(), objects.end(), compareObjects);

  for (int i=0;i<objects.size();i++) {
    if (!objects[i]->num_properties["hidden"])
      objects[i]->render();
  }

  glPopMatrix();

  for (int i=0;i<dialogs.size();i++) {
    dialogs[i]->render();
  }
}
