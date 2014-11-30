#include "map.h"
using namespace bazinga;

Layer::Layer (BjObject *jLayer) {
  BjValue* jActive = jLayer->get("active");

  if (jActive->type == BjValue::jTrue) {
    active = true;
  } else
  if (jActive->type == BjValue::jFalse) {
    active = false;
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
        objects.push_back(Object(jOValue->object));
      } else {
        // Error
      }
    }
  } else {
    // Error
  }

  cout << objects.size() << " objects loaded" << endl;
}

void Layer::update () {
  for (int i=0;i<objects.size();i++) {
    objects[i].update();
  }
}

Map::Map (BjObject *jMap) {
  // Get an array called "layers" and iterate over it
  BjValue* jLayers = jMap->get("layers");

  // Is it an array?
  if (jLayers->type == BjValue::jArray) {
    BjArray *array = jLayers->array;
    int i;

    for (i=0;i<array->array.size();i++) {
      BjValue *jOValue = array->array[i];

      if (jOValue->type == BjValue::jObject) {
        layers.push_back(Layer(jOValue->object));
      } else {
        // Error
      }
    }
  } else {
    // Some kind of error
  }

  cout << layers.size() << " layers loaded" << endl;
}

void Map::update() {
  for (int i=0;i<layers.size();i++) {
    layers[i].update();
  }
}
