#ifndef BAZINGA_IMAGE_H
#define BAZINGA_IMAGE_H

#include "filesystem.h"
#include <SDL_opengl.h>

namespace bazinga {
	class Image {
	public:
	    unsigned int w,h;
	    unsigned int rw, rh;
	    GLuint id;

		Image(Path);
		~Image();

		void render(int, int);
	};
}

#endif /* BAZINGA_IMAGE_H */