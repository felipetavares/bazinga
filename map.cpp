#include "map.h"
#include "bazinga.h"
using namespace bazinga;

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
        map->addObject(new Object(jOValue->object, layer));
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

Map::Map () {
  cout << "bazinga: chipmunk: initializing space...";

  pSpace = cpSpaceNew();

  if (pSpace) {
    cout << " ok" << endl;
  } else {
    cout << " fail" << endl;
  }
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

  cout << objects.size() << " objects loaded" << endl;
}

Map::~Map () {
  if (pSpace)
    cpSpaceFree(pSpace);
}

cpSpace* Map::getSpace () {
  return pSpace;
}

void Map::addObject (Object *object) {
  objects.push_back(object);
}

void Map::update() {
  for (int i=0;i<objects.size();i++) {
    objects[i]->update();
  }

  cpSpaceStep(pSpace, bazinga::delta);
}

void Map::render() {
  sort (objects.begin(), objects.end(), compareObjects);

  for (int i=0;i<objects.size();i++) {
    objects[i]->render();
  }
}
