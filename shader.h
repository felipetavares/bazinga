#ifndef BAZINGA_SHADER_H
#define BAZINGA_SHADER_H

#include "filesystem.h"
#include <SDL_opengl.h>
#include <map>
using namespace std;

namespace bazinga {
	namespace shader {
		class Vertex {
			GLuint id;
		public:
			Vertex(Path);
			~Vertex();

			GLuint getID();
		};

		class Fragment {
			GLuint id;
		public:
			Fragment(Path);
			~Fragment();

			GLuint getID();
		};

		class Program {
			GLuint id;

			map <string, GLint> uniformLocations;
		public:
			Program(Vertex*, Fragment*);
			~Program();
			
			void use();
			void stopUse();
			void loadUniforms(vector<string>);

			// 1i
			void setUniform(string, GLint);
			// 4f
			void setUniform(string, GLfloat, GLfloat, GLfloat, GLfloat);
		};
	
		void showInfoLog(GLuint);
		void showProgramInfoLog(GLuint);
	}
}

#endif /* BAZINGA_SHADER_H */