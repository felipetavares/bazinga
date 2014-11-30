#ifndef BAZINGA_MAP_H
#define BAZINGA_MAP_H

#include "object.h"
#include "json.h"

namespace bazinga {
  class Layer {
    bool active;
    vector <Object> objects;
  public:
    Layer (BjObject*);

    void update();
  };

  class Map {
    vector <Layer> layers;
  public:
    Map (BjObject*);

    void update();
  };
}

#endif /* BAZINGA_MAP_H */
