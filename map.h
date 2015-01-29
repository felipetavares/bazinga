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

  class Dialog {
    string text;
    int id;
  public:
    Dialog(int, string);
    void update();
    void render();
    int getID();
  };

  class Map {
    vector <Object*> objects;
    static int oid;
    static int did;
    // Tudo relacionado à física
    // tem como prefixo 'p'
    cpSpace *pSpace = NULL;

    float camx, camy;

    vector <Dialog*> dialogs;
  public:
    Map ();
    ~Map();
    void init (BjObject *);

    int newObject (lua_State*);
    void addObject(Object*);

    void deleteObject(int);

    void render();
    void update();
    int newDialog(string);
    void deleteDialog(int);
    int getNewID();

    void setCamera (float, float);

    cpSpace *getSpace();
  };
}

#endif /* BAZINGA_MAP_H */
