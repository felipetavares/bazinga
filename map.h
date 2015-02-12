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
    static const int bufferSize = 3;
    int id;

    string text;
    int textPosition;

    string buffer[bufferSize];
    int bufferPosition;
  
    int sizeW;

    float nextTime;
    float intervalTime;
    float nextWordSize;
    bool ended;
  public:
    Dialog(int, string);
    void update();
    void render();
    int getID();
    bool isEnded();
  private:
    // Add a character to the output
    void fillChar();
    // Move to next output line
    void nextLine();
  };

  class Map {
    vector <Object*> objects;
    static int oid;
    static int did;
    // Tudo relacionado à física
    // tem como prefixo 'p'
    cpSpace *pSpace = NULL;

    float camx, camy;
    float zoomX, zoomY;

    bool reorder = true;

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
    int searchObject(string);
    void hideObject(int, bool);
    bool isDialogEnded(int);
    void deleteDialog(int);
    void setReorder(bool);
    void setGravity(float, float);
    int getNewID();

    void setCamera (float, float);
    void setZoom (float, float);

    cpSpace *getSpace();
  };
}

#endif /* BAZINGA_MAP_H */
