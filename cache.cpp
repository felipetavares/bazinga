#include "cache.h"
#include "console.h"
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
        console << LINEINFO << "loading " << path.getPath() << outline;

        Image *image = new Image(path);
        cachedData[path.getPath()] = image;

        return image;
      }
    }
  
    shader::Vertex* getVertexShader (Path path) {
      try {
        return cachedVertex.at(path.getPath());
      } catch (exception e) {
        console << LINEINFO << "loading " << path.getPath() << outline;

        shader::Vertex *shader = new shader::Vertex(path);
        cachedVertex[path.getPath()] = shader;

        return shader;
      }
    }

    shader::Fragment* getFragmentShader (Path path) {
      try {
        return cachedFragment.at(path.getPath());
      } catch (exception e) {
        console << LINEINFO << "loading " << path.getPath() << outline;

        shader::Fragment *shader = new shader::Fragment(path);
        cachedFragment[path.getPath()] = shader;

        return shader;
      }      
    }

    shader::Program* getShaderProgram (string name) {
      try {
        return cachedProgram.at(name);
      } catch (exception e) {
        console << LINEINFO << "shader program not yet created " << name << outline;
        return NULL;
      }      
    }

    text::Font* getFont (string name) {
      try {
        return cachedFont.at(name);
      } catch (exception e) {
        console << LINEINFO << "font not yet created '" << name << "'" << outline;
        return NULL;
      }
    }

    void createFont (Path fname, string name) {
      try {
        cachedFont.at(name);
      } catch (exception e) {
        cachedFont[name] = new text::Font(fname);
      }
    }

    void createShaderProgram (shader::Vertex* vert, shader::Fragment* frag, string name) {
      cachedProgram[name] = new shader::Program(vert, frag);
    }

    void init () {
      console << LINEINFO << "init()" << outline;
    }

    void deinit () {
      map <string, Image*>::iterator i;
      for (i=cachedData.begin();i != cachedData.end(); i++) {
        console << LINEINFO << "deleting " << i->first << outline;
        delete i->second;
      }

      for (auto i :cachedVertex) {
        console << LINEINFO << "deleting " << i.first << outline;
        delete i.second;
      }

      for (auto i :cachedFragment) {
        console << LINEINFO << "deleting " << i.first << outline;
        delete i.second;
      }

      for (auto i :cachedProgram) {
        console << LINEINFO << "deleting shader program '" << i.first << "'" << outline;
        delete i.second;
      }

      for (auto i :cachedFont) {
        console << LINEINFO << "deleting font '" << i.first << "'" << outline;
        delete i.second;
      }
    }
  }
}
