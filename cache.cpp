#include "cache.h"
#include "console.h"
#include <exception>
#include <iostream>

namespace bazinga {
  namespace cache {
    map <string, Image*> cachedImage;
    map <string, shader::Vertex*> cachedVertex;
    map <string, shader::Fragment*> cachedFragment;
    map <string, shader::Program*> cachedProgram;
    map <string, text::Font*> cachedFont;
    map <string, audio::AudioData*> cachedAudio;

    audio::AudioData* getAudio (Path path) {
      path = Path(bazinga::projectPath+path.getPath());

      try {
        return cachedAudio.at(path.getPath());
      } catch (exception e) {
        console << LINEINFO << "loading " << path.getPath() << outline;

        audio::AudioData *audio = new audio::AudioData(path);
        cachedAudio[path.getPath()] = audio;

        return audio;
      }
    }

    Image* getTexture (Path path) {
      path = Path(bazinga::projectPath+path.getPath());

      try {
        return cachedImage.at(path.getPath());
      } catch (exception e) {
        console << LINEINFO << "loading " << path.getPath() << outline;

        Image *image = new Image(path);
        cachedImage[path.getPath()] = image;

        return image;
      }
    }

    shader::Vertex* getVertexShader (Path path) {
      path = Path(bazinga::projectPath+path.getPath());

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
      path = Path(bazinga::projectPath+path.getPath());

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
      fname = Path(bazinga::projectPath+fname.getPath());

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
      for (i=cachedImage.begin();i != cachedImage.end(); i++) {
        console << LINEINFO << "deleting " << i->first << outline;
        delete i->second;
      }
      cachedImage.clear();

      for (auto i :cachedVertex) {
        console << LINEINFO << "deleting " << i.first << outline;
        delete i.second;
      }
      cachedVertex.clear();

      for (auto i :cachedFragment) {
        console << LINEINFO << "deleting " << i.first << outline;
        delete i.second;
      }
      cachedFragment.clear();

      for (auto i :cachedProgram) {
        console << LINEINFO << "deleting shader program '" << i.first << "'" << outline;
        delete i.second;
      }
      cachedProgram.clear();

      for (auto i :cachedFont) {
        console << LINEINFO << "deleting font '" << i.first << "'" << outline;
        delete i.second;
      }
      cachedFont.clear();

      for (auto i :cachedAudio) {
        console << LINEINFO << "deleting font '" << i.first << "'" << outline;
        delete i.second;
      }
      cachedAudio.clear();
    }
  }
}
