#include "cache.h"
#include <exception>
#include <iostream>

namespace bazinga {
  namespace cache {
    map <string, Image*> cachedData;
    map <string, shader::Vertex*> cachedVertex;
    map <string, shader::Fragment*> cachedFragment;
    map <string, shader::Program*> cachedProgram;
    map <string, text::Font*> cachedFont;

    Image* getTexture (Path path) {
      try {
        return cachedData.at(path.getPath());
      } catch (exception e) {
        cout << "bazinga: cache: loading " << path.getPath() << endl;

        Image *image = new Image(path);
        cachedData[path.getPath()] = image;

        return image;
      }
    }
  
    shader::Vertex* getVertexShader (Path path) {
      try {
        return cachedVertex.at(path.getPath());
      } catch (exception e) {
        cout << "bazinga: cache: loading " << path.getPath() << endl;

        shader::Vertex *shader = new shader::Vertex(path);
        cachedVertex[path.getPath()] = shader;

        return shader;
      }
    }

    shader::Fragment* getFragmentShader (Path path) {
      try {
        return cachedFragment.at(path.getPath());
      } catch (exception e) {
        cout << "bazinga: cache: loading " << path.getPath() << endl;

        shader::Fragment *shader = new shader::Fragment(path);
        cachedFragment[path.getPath()] = shader;

        return shader;
      }      
    }

    shader::Program* getShaderProgram (string name) {
      try {
        return cachedProgram.at(name);
      } catch (exception e) {
        cout << "bazinga: cache: shader program not yet created " << name << endl;
        return NULL;
      }      
    }

    text::Font* getFont (string name) {
      try {
        return cachedFont.at(name);
      } catch (exception e) {
        cout << "bazinga: cache: font not yet created '" << name << "'" << endl;
        return NULL;
      }
    }

    void createFont (Path fname, string name) {
      if (cachedFont[name] == NULL)
        cachedFont[name] = new text::Font(fname);
    }

    void createShaderProgram (shader::Vertex* vert, shader::Fragment* frag, string name) {
      cachedProgram[name] = new shader::Program(vert, frag);
    }

    void init () {
      cout << "bazinga: cache: init()" << endl;
    }

    void deinit () {
      map <string, Image*>::iterator i;
      for (i=cachedData.begin();i != cachedData.end(); i++) {
        cout << "bazinga: cache: deleting " << i->first << endl;
        delete i->second;
      }

      for (auto i :cachedVertex) {
        cout << "bazinga: cache: deleting " << i.first << endl;
        delete i.second;
      }

      for (auto i :cachedFragment) {
        cout << "bazinga: cache: deleting " << i.first << endl;
        delete i.second;
      }

      for (auto i :cachedProgram) {
        cout << "bazinga: cache: deleting shader program '" << i.first << "'" << endl;
        delete i.second;
      }

      for (auto i :cachedFont) {
        cout << "bazinga: cache: deleting font '" << i.first << "'" << endl;
        delete i.second;
      }
    }
  }
}
