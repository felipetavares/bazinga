#include "map.h"
#include "bazinga.h"
#include "text.h"
#include "image.h"
#include "cache.h"
#include "console.h"
#include "editor.h"
#include "gui.h"
#include <ctype.h>
#include <thread>
using namespace bazinga;

Dialog::Dialog (int id, string text, Path path, string fontName):
  imagePath(path), id(id), fontName(fontName),
  text(text), textPosition(0), bufferPosition(0),
  nextTime(0), ended(false) {
    sizeW = video::windowWidth/2;
    intervalTime = 0.05;
    nextWordSize = 0;
}

void Dialog::update () {
  if (textPosition < text.size()) {
    if (curtime > nextTime) {
      switch (text[textPosition]) {
        case '!': case '.': case '?':
          intervalTime = 1;
        break;
        case ';': case ':':
          intervalTime = 0.5;
        break;
        case ',':
          intervalTime = 0.25;
        break;
        default:
          intervalTime = 0.05;
      }

      nextTime = curtime+intervalTime;
      fillChar();
    }
  } else {
    ended = true;
  }
}

void Dialog::render () {
  auto size = text::measureText("#");

  int startY = video::windowHeight/2-1.5*size.h*bufferSize;

  Image *img = cache::getTexture(imagePath);

  img->render(-sizeW/2+img->w/2, startY-20-img->h/2-10);

  video::setColor1(video::Color(0, 0, 0, 0.5));
  video::fillRect(-sizeW/2, startY-20, sizeW+20, 1.5*size.h*bufferSize+10);
  video::setColor1(video::Color(1, 1, 1, 1));
  video::fillRect(-sizeW/2-10, startY-30, sizeW+20, 1.5*size.h*bufferSize+10);

  auto font = cache::getFont(fontName);
  text::setFont(font);
  text::setAlign(text::Left);
  text::setBaseline(text::Alphabetic);

  for (int i=0;i<bufferSize;i++)
    text::fillText(buffer[i], -sizeW/2, startY+i*1.5*size.h-10);
}

void Dialog::fillChar () {
  audio::play(Path("assets/audio/keyhit.ogg"));

  buffer[bufferPosition] += text[textPosition++];

  auto size = text::measureText(buffer[bufferPosition]);

  if (isspace(text[textPosition])) {
    if (size.w+nextWordSize >= sizeW) {
      while (isspace(text[textPosition]) && textPosition<text.size())
        textPosition++;
      nextLine();
    }

    nextWord = "";
    int i = textPosition;
    while (isspace(text[i]) && i<text.size())
      i++;
    for (;i<text.size() && !isspace(text[i]);i++);
    while (isspace(text[i]) && i<text.size())
      i++;
    for (;i<text.size() && !isspace(text[i]);i++) {
      nextWord += text[i];
    }

    nextWordSize = text::measureText(nextWord).w;
  }
}

void Dialog::nextLine () {
  if (bufferPosition < bufferSize-1)
    bufferPosition++;
  else {
    for (int i=0;i<bufferSize-1;i++) {
      buffer[i] = buffer[i+1];
    }

    buffer[bufferSize-1] = "";
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

Map::Map (Path file):
  file(file),
  edit(NULL) {
  console << LINEINFO << "initializing space... ";

  pSpace = cpSpaceNew();
  cpSpaceSetDamping (pSpace, 0.01);

  if (pSpace) {
    console << " ok" << outline;
  } else {
    console << " fail" << outline;
  }

  console << LINEINFO << "initializing collision handler..." << outline;

  cpSpaceSetDefaultCollisionHandler(pSpace, pBeginCollision, NULL, NULL, NULL, NULL);

  zoomX = 1;
  zoomY = 1;
}

int Map::getNewID() {
  return oid++;
}

bool Map::init () {
  lScript = luaL_newstate();
  luaL_openlibs (lScript);
  Object::createLuaAPI(lScript);

  // Loads the file
  if (luaL_dofile(lScript, (Path("scripts/"+file.getName()+".lua")).getPath().c_str())) {
    console << LINEINFO << "script contains errors" <<  outline;

    if (lua_isstring(lScript, -1)) {
      console << "\t" << lua_tostring(lScript, -1) << outline;
    }

    lua_close(lScript);
    lScript = NULL;
  }

  edit = new editor::Editor();

  BjObject *jMap;

  if (fs::fileExists(file)) {
    char *data = fs::getFileData(file);
    string sData = string (data);

    jMap = json::parse (sData);

    delete data;
  } else {
    console << LINEINFO << "could not load " << file.getPath() << outline;
    console << "\t" << "aborting" << outline;
    return false;
  }

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

  console << LINEINFO << "" << objects.size() << " objects loaded" << outline;

  delete jMap;
  return true;
}

Map::~Map () {
  if (pSpace)
    cpSpaceFree(pSpace);
  if (edit)
    delete edit;
}

int Map::newDialog (string text, Path path, string font) {
  int dialogID = did++;

  dialogs.push_back(new Dialog(dialogID, text, path, font));

  return dialogID;
}

void Map::deleteDialog (int id) {
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
  for (int i=0;i<dialogs.size();i++) {
    auto d = dialogs[i];
    if (d->getID() == id) {
      return d->isEnded();
    }
  }

  return true;
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
  ((Object*)(long)id)->num_properties["delete"] = 1;
}

int Map::searchObject (string name) {
  for (auto i=objects.begin();i!=objects.end();i++) {
    if ((*i)->str_properties["name"] == name) {
      return (int)((long)(*i));
    }
  }

  return 0;
}

void Map::hideObject (int id, bool hide) {
  ((Object*)(long)id)->num_properties["hidden"] = (int)hide;
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

// Creates an object from an object description file
// NOT Called from Lua
void Map::newObject (Path fpath) {
  char *data = fs::getFileData(fpath);
  string sData = string (data);

  BjObject *object = json::parse (sData);

  Object *o = new Object(object, 0);

  o->init();
  addObject(o);

  delete object;
  delete data;
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

bool Map::getPropertie(int id, string prop, string& str, float& num) {
  try {
      str = ((Object*)(long)id)->str_properties.at(prop);
      return true;
  } catch (exception) {
    try {
      num = ((Object*)(long)id)->num_properties.at(prop);
      return false;
    } catch (exception) {
      num = 0;
      return false;
    }
  }
}

void Map::setPropertie(int id, string prop, float num) {
  ((Object*)(long)id)->num_properties[prop] = num;
}

void Map::setPropertie(int id, string prop, string str) {
  ((Object*)(long)id)->str_properties[prop] = str;
}

cpBool Map::pmBeginCollision (cpArbiter* arb, cpSpace* space, void* data) {
    //cpSpaceAddPostStepCallback(space, (cpPostStepFunc)postStepAct, arb, NULL);

    cpShape *a, *b;
    cpArbiterGetShapes(arb, &a, &b);

    lua_getglobal(lScript, "collision_begin");

    ((Object*)cpShapeGetUserData(a))->createLuaProperties(lScript);
    ((Object*)cpShapeGetUserData(b))->createLuaProperties(lScript);

    if (lua_pcall(lScript, 2, 1, 0)) {
      console << LINEINFO << "error when calling collision_begin() in scene script" << outline;

      if (lua_isstring(lScript, -1)) {
        console << "\t" << lua_tostring(lScript, -1) << outline;
      }
    } else {
      if (lua_gettop(lScript) < 1) { // This condition is not working ok
        console << LINEINFO << "collision_begin() in scene script doesn't return value" << outline;
      } else {
        return lua_toboolean(lScript, -1);
      }
    }

    // Default: collision will occur
    return true;
}

void Map::update() {
  for (auto i=objects.begin();i!=objects.end();i++) {
    if ((*i)  ->num_properties["delete"]) {
      delete (*i);
      objects.erase(i);
      i--;
    } else
      (*i)->update();
  }

  for (int i=0;i<dialogs.size();i++) {
    dialogs[i]->update();
  }

  cpSpaceStep(pSpace, bazinga::delta);
}

void Map::render() {
  glClearColor (video::backgroundColor.r,
                video::backgroundColor.g,
                video::backgroundColor.b,
                video::backgroundColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glPushMatrix();
  glScalef(zoomX, zoomY, 0);

  glTranslatef (-camx, -camy, 0);

  if (reorder)
    sort (objects.begin(), objects.end(), compareObjects);

  for (auto o :objects) {
    o->render();
  }

  edit->render();

  glPopMatrix();

  for (int i=0;i<dialogs.size();i++) {
    dialogs[i]->render();
  }
}

int Map::getObjectCount () {
  return objects.size();
}

void Map::mousemove(int x, int y) {
  if (edit)
    edit->drag(vec2(x/zoomX, y/zoomY));
}

void Map::mousepress(int bt, int x, int y) {
  if (bt == 1) {
    bool selected = false;

    for (int i=objects.size()-1;i>=0;i--) {
      auto o = objects[i];
      if (gui::inside(x, y, (o->num_properties["x"]-camx)*zoomX, (o->num_properties["y"]-camy)*zoomY,o->num_properties["w"]*zoomX, o->num_properties["h"]*zoomY)) {
        editor::openPropertiesWindow(o);
        if (edit) {
          edit->select(o);
        }
        selected = true;
        break;
      }
    }

    if (!selected) {
      if (edit) {
        edit->select(NULL);
      }
    }

    if (edit) {
      edit->setDrag(true, vec2(x/zoomX, y/zoomY));
    }
  }
}

void Map::mouseunpress(int, int, int) {
  if (edit)
    edit->setDrag(false);
}

BjObject* Map::toJSON(float *progress) {
  BjArray *layers = new BjArray();
  BjObject *map = new BjObject();
  BjObject *layer = new BjObject();
  BjArray *array = new BjArray();

  map->keys.push_back("layers");
  map->values.push_back(new BjValue(layers));

  layer->keys.push_back("active");
  layer->values.push_back(new BjValue(true));

  layer->keys.push_back("data");
  layer->values.push_back(new BjValue(array));

  layers->array.push_back(new BjValue(layer));

  int i = 0;
  for (auto o :objects) {
    array->array.push_back(new BjValue(o->toJSON()));

    *progress = float(i)/float(objects.size());
    i++;
  }

  return map;
}

void Map::save () {
  auto fileName = file;

  auto progress = new float(0);
  editor::openProgressWindow(progress);

  thread t ([this, progress, fileName] () {
    auto data = toJSON(progress);
    auto dump = json::dumpjson(data);

    fs::setFileData(fileName, dump.c_str(), dump.size());

    delete data;

    *progress = 1;
  });

  t.detach();
}
