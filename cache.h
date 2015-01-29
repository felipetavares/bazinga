#ifndef BAZINGA_CACHE_H
#define BAZINGA_CACHE_H

#include "shader.h"
#include "image.h"
#include "text.h"
#include "filesystem.h"
#include <map>
using namespace std;

namespace bazinga {
  namespace cache {
  	void init();
  	void deinit();

    Image* getTexture (Path);
  	shader::Vertex* getVertexShader(Path);
  	shader::Fragment* getFragmentShader(Path);
  	shader::Program* getShaderProgram(string);
  	text::Font* getFont(string);

  	void createShaderProgram(shader::Vertex*, shader::Fragment*, string);
  	void createFont (Path, string);
  }
}

#endif /* BAZINGA_CACHE_H */
