#ifndef BAZINGA_MAP_H
#define BAZINGA_MAP_H

#include "object.h"
#include "json.h"

namespace bazinga {
  class Map;

  class Layer {
  public:
    Layer (BjObject*, Map*, int);
    ~Layer();
  };

  class Map {
    vector <Object*> objects;
  public:
    Map (BjObject*);

    void addObject(Object*);

    void render();
    void update();
  };
}

#endif /* BAZINGA_MAP_H */
