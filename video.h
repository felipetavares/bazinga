#ifndef BAZINGA_VIDEO_H
#define BAZINGA_VIDEO_H

#include <iostream>
#include "filesystem.h"
#include "map.h"
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
using namespace std;

namespace bazinga {
	class video {
		static vector <SDL_Rect> *videoModes;
		static bool anyResolution;

		static SDL_Surface *screen;

		static const Uint32 videoFlags = /*SDL_FULLSCREEN |*/ SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF;
		static Path icon;
	public:
		class Image {
		public:
            unsigned int w,h;
            unsigned int rw, rh;
            GLuint id;

			Image(Path);
		};

		static int windowBpp;
		static int windowWidth;
		static int windowHeight;

		video();
		~video();

		static void init();
		static void deinit();

		static void setWindowTitleAndIcon (string,string="");
		static void render ();
		static void renderMap (Map*);
	private:
		static void getVideoModes ();
		static void findBestVideoMode ();
	};
}

#endif /* BAZINGA_VIDEO_H */
