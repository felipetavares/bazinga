#include "cache.h"
#include <exception>
#include <iostream>

namespace bazinga {
  namespace cache {
    map <string, GLuint> cachedData;

    GLuint getTexture (Path path) {
      try {
        return cachedData.at(path.getPath());
      } catch (exception e) {
        cout << "bazinga: cache: " << path.getPath() << endl;

        video::Image image = video::Image(path);
        cachedData[path.getPath()] = image.getId();
      }
    }
  }
}
