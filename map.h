#ifndef BAZINGA_MAP_H
#define BAZINGA_MAP_H

#include "object.h"
#include "json.h"
#include <chipmunk.h>

namespace bazinga {
  class Map;

  class Layer {
  public:
    Layer (BjObject*, Map*, int);
    ~Layer();
  };

  class Map {
    vector <Object*> objects;
    
    // Todas as coisas relacionadas à física
    // tem como prefixo 'p'
    cpSpace *pSpace = NULL;
  public:
    Map ();
    ~Map();
    void init (BjObject *);

    void addObject(Object*);

    void render();
    void update();

    cpSpace *getSpace();
  };
}

#endif /* BAZINGA_MAP_H */
