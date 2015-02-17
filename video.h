#ifndef BAZINGA_VIDEO_H
#define BAZINGA_VIDEO_H

#include <iostream>
#include "filesystem.h"
#include "map.h"
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <functional>
using namespace std;

namespace bazinga {
	class video {
		static vector <SDL_Rect> *videoModes;
		static bool anyResolution;

		static SDL_Surface *screen;

		static const Uint32 videoFlags = SDL_FULLSCREEN | SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF;
		static Path icon;

		static float cr, cg, cb, ca;
		static float bcr, bcg, bcb;
		static float fcr, fcg, fcb;
		static float endtime,starttime;
		static bool fadedirection;
	public:
		static function <void(void)> onFinish;
		static int windowBpp;
		static int windowWidth;
		static int windowHeight;

		video();
		~video();

		static void init();
		static void deinit();

		static void setWindowTitleAndIcon (string,string="");
		static void setWindowIcon(Path);
		static void render ();
		static void renderMap (Map*);
	
		static void fillRect(int, int, int, int);
		static void strokeRect(int, int, int, int);
		static void fillCircle(int, int, int);
		static void strokeCircle(int, int, int);
		static void setColor(float, float, float, float);
		static void setBackgroundColor(float, float, float);
		static void fadeFrom(float, float, float, float);
		static void fadeTo(float, float, float, float);
	private:
		static void getVideoModes ();
		static void findBestVideoMode ();
	};
}

#endif /* BAZINGA_VIDEO_H */
