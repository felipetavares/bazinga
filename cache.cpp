#include "cache.h"
#include <exception>
#include <iostream>

namespace bazinga {
  namespace cache {
    map <string, video::Image*> cachedData;

    video::Image getTexture (Path path) {
      try {
        return *cachedData.at(path.getPath());
      } catch (exception e) {
        cout << "bazinga: cache: " << path.getPath() << endl;

        video::Image *image = new video::Image(path);
        cachedData[path.getPath()] = image;
      }
    }
  }
}
