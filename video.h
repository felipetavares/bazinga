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
	public:
		class Color {
		public:
			float r,g,b,a;

			Color(float,float,float,float=1);
		};
	private:
		static vector <SDL_Rect> *videoModes;
		static bool anyResolution;

		static SDL_Surface *screen;

		static Uint32 videoFlags;
		static Path icon;

		static Color c1;
		static Color c2;
		static Color backgroundColor;
		static Color foregroundColor;
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
		static void shadow(int,int,int,int,int);
		static void fillVGradient(int, int, int, int);
		static void fillHGradient(int, int, int, int);
		static void fillRect(int, int, int, int);
		static void strokeRect(int, int, int, int);
		static void fillCircle(int, int, int);
		static void strokeCircle(int, int, int);
		
		static void setColor1(Color);
		static void setColor2(Color);
		static void setBackgroundColor(Color);
		
		static void fadeFrom(Color, float);
		static void fadeTo(Color, float);
	private:
		static void getVideoModes ();
		static void findBestVideoMode ();
	};
}

#endif /* BAZINGA_VIDEO_H */
