#include "video.h"
#include "bazinga.h"
#include "console.h"
#include <cmath>
#include <il.h>
using namespace bazinga;

video::Color video::c1 = video::Color(0, 0, 0, 0);
video::Color video::c2 = video::Color(0, 0, 0, 0);
video::Color video::foregroundColor = video::Color(0, 0, 0, 0);
video::Color video::backgroundColor = video::Color(0, 0, 0, 0);
Uint32 video::videoFlags;
float video::endtime = 0, video::starttime = 0;
bool video::fadedirection;
int video::windowBpp = 0;
int video::windowWidth = 0;
int video::windowHeight = 0;
bool video::anyResolution = false;
function <void(void)> video::onFinish = NULL;
vector <SDL_Rect> *video::videoModes = NULL;
Path video::icon;
SDL_Surface *video::screen = NULL;

video::Color::Color (float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

video::video () {
	endtime = -1;
	starttime = -1;
}

video::~video () {
}

void video::resize (int w, int h) {
	windowWidth = w;
	windowHeight = h;

	//screen = SDL_SetVideoMode (windowWidth,windowHeight,windowBpp,videoFlags);
	glViewport( -windowWidth/2, windowWidth/2, windowHeight/2, -windowHeight/2);

	glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho(-windowWidth/2, windowWidth/2, windowHeight/2, -windowHeight/2, -100.0f, 100.0f);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	console << LINEINFO << "OpenGL: " << string((char*)glGetString(GL_VERSION)) << outline;
}

void video::init() {
	videoFlags = /*SDL_FULLSCREEN |*/ SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF /*| SDL_RESIZABLE*/;

	if (SDL_Init (SDL_INIT_EVERYTHING) < 0) {
		console << LINEINFO << "cannot Init SDL" << outline;
		exit (-1);
	}

	auto iconPath = Path("assets/images/icon.bmp");
	console << LINEINFO << "using " << iconPath.getPath() << " as icon" << outline;
    bazinga::video::setWindowIcon(iconPath);

	SDL_EnableUNICODE(1);

	ilInit();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// Enable VSync
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	//SDL_GL_SetAttribute(SDL_GL_SAMPLE_BUFFERS_ARB, 1);
	//SDL_GL_SetAttribute(SDL_GL_SAMPLES_ARB, 2);

	anyResolution = false;
	// Initialize video modes array
	videoModes = new vector <SDL_Rect>();
	getVideoModes();

	findBestVideoMode();

	screen = SDL_SetVideoMode (windowWidth,windowHeight,windowBpp,videoFlags);
	SDL_ShowCursor (0);

	console << LINEINFO << "window (" << windowWidth <<  ", " << windowHeight << ")" << outline;

	// CHANGE THAT IF NEED BE
	//glEnable (GL_DEPTH_TEST);

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA );
	// Antialiased polygons
	//glEnable( GL_MULTISAMPLE );
	// Antialiasing
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	//glEnable( GL_POLYGON_SMOOTH );

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glAlphaFunc(GL_GREATER,0.5f);
	//glEnable(GL_ALPHA_TEST);

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glViewport( -windowWidth/2, windowWidth/2, windowHeight/2, -windowHeight/2);

	glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho(-windowWidth/2, windowWidth/2, windowHeight/2, -windowHeight/2, -100.0f, 100.0f);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	console << LINEINFO << "OpenGL: " << string((char*)glGetString(GL_VERSION)) << outline;
}

void video::deinit () {
	delete videoModes;
}

void video::getVideoModes () {
	SDL_Rect **modeList;

	console << LINEINFO << "querying video modes" << outline;

	modeList = SDL_ListModes (NULL, videoFlags);

	console << LINEINFO << "loaded video modes from SDL" << outline;

	if (modeList != NULL) {
		if (modeList == (SDL_Rect**)-1) {
			console << LINEINFO << "all resolutions available" << outline;
			anyResolution = true;
		} else {
			for (int i=0;modeList[i];i++) {
				videoModes->push_back (*modeList[i]);
			}
		}
	} else {
		console << LINEINFO << "no video modes found" << outline;
		exit (-1);
	}

	SDL_WM_GrabInput(SDL_GRAB_OFF); // Allow the WM to execute it's default keys
}

void video::findBestVideoMode () {
	// Bits per pixel
	windowBpp = SDL_GetVideoInfo()->vfmt->BitsPerPixel;

	// Resolution
	windowWidth = windowHeight = 0;

	if (anyResolution) { // Any resolution available
		windowWidth = 1280;
		windowHeight = 720;
	} else {
		for (auto rect : (*videoModes) ) {
			if (rect.w * rect.h > windowHeight * windowWidth) {
				windowWidth = rect.w;
				windowHeight = rect.h;
			}
		}
	}
}

void video::setWindowTitleAndIcon (	string _title,
										string _icon) {
	SDL_WM_SetCaption (_title.c_str(),_icon.c_str());
}

void video::setWindowIcon (Path icon) {
	SDL_Surface* iconI = SDL_LoadBMP(icon.getPath().c_str());
	SDL_SetColorKey (iconI, SDL_SRCCOLORKEY, SDL_MapRGB(iconI->format, 255, 0, 0));

	SDL_WM_SetIcon(iconI, NULL);
}

void video::setColor1 (Color c1) {
	video::c1 = c1;
}

void video::setColor2 (Color c2) {
	video::c2 = c2;
}

void video::fillVGradient (int x, int y, int w, int h) {
	glPushMatrix();
		glTranslatef (x, y, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(0, 0, 0);
			glVertex3f(w, 0, 0);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(w, h, 0);
			glVertex3f(0, h, 0);
		glEnd();
	glPopMatrix();
}

void video::fillHGradient (int x, int y, int w, int h) {
	glPushMatrix();
		glTranslatef (x, y, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(w, 0, 0);
			glVertex3f(w, h, 0);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(0, h, 0);
			glVertex3f(0, 0, 0);
		glEnd();
	glPopMatrix();
}

void video::setBackgroundColor (Color backgroundColor) {
	video::backgroundColor = backgroundColor;
}

void video::fillRect (int x, int y, int w, int h) {
	glPushMatrix();
		glColor4f(c1.r,c1.g,c1.b,c1.a);
		glTranslatef (x, y, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glVertex3f(0, 0, 0);
			glVertex3f(w, 0, 0);
			glVertex3f(w, h, 0);
			glVertex3f(0, h, 0);
		glEnd();
	glPopMatrix();
}

void video::fillCircle (int cx, int cy, int r) {
	int num_segments = 3*r;
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta); // Precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r; // We start at angle = 0
	float y = 0;

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(c1.r,c1.g,c1.b,c1.a);
	for(int i = 0; i < num_segments; i++) {
		glVertex2f(x + cx, y + cy); // Output vertex

		// Apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

void video::strokeRect (int x, int y, int w, int h) {
	glPushMatrix();
		glColor4f(c1.r,c1.g,c1.b,c1.a);
		glTranslatef (x, y, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex3f(0, 0, 0);
			glVertex3f(w, 0, 0);
			glVertex3f(w, h, 0);
			glVertex3f(0, h, 0);
		glEnd();
	glPopMatrix();
}

void video::strokeCircle (int cx, int cy, int r) {
	int num_segments = 3*r;
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta); // Precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = r; // We start at angle = 0
	float y = 0;

	glBegin(GL_LINES);
	glColor4f(c1.r,c1.g,c1.b,c1.a);
	for(int i = 0; i < num_segments; i++) {
		glVertex2f(x + cx, y + cy); // Output vertex

		// Apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

void video::render () {
	if (endtime > 0) {
		float factor;

		if (fadedirection)
			factor = 1-(bazinga::curtime-starttime)/(endtime-starttime);
		else
			factor = (bazinga::curtime-starttime)/(endtime-starttime);

		if (bazinga::curtime > endtime) {
			endtime = starttime = -1;

			if (onFinish) {
				onFinish();
				onFinish = NULL;
			}
		}

		video::setColor1(video::Color(foregroundColor.r,foregroundColor.g,foregroundColor.b, factor));
		video::fillRect(-windowWidth/2, -windowHeight/2, windowWidth, windowHeight);
	}

	SDL_GL_SwapBuffers();
}

void video::fadeFrom (Color foregroundColor, float time) {
	video::foregroundColor = foregroundColor;

	endtime = bazinga::curtime+time;
	starttime = bazinga::curtime;
	fadedirection = true;
}

void video::fadeTo (Color foregroundColor, float time) {
	video::foregroundColor = foregroundColor;

	endtime = bazinga::curtime+time;
	starttime = bazinga::curtime;
	fadedirection = false;
}

void video::shadow (int x, int y, int w, int h, int size) {
	// Compute shadow's bounding box
	int x2 = x - size;
	int y2 = y - size;
	int w2 = w + size*2;
	int h2 = h + size*2;

	glPushMatrix();
		glTranslatef (x2, y2, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(0, 0, 0);
			glVertex3f(w2, 0, 0);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(w+size, size, 0);
			glVertex3f(size, size, 0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef (x2, y2, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(size, h+size, 0);
			glVertex3f(size+w, h+size, 0);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(w2, h2, 0);
			glVertex3f(0, h2, 0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef (x2, y2, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(w+size, size, 0);
			glVertex3f(w+size, h+size, 0);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(w2, h2, 0);
			glVertex3f(w2, 0, 0);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef (x2, y2, 0);
		glDisable (GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glColor4f(c2.r,c2.g,c2.b,c2.a);
			glVertex3f(size, size, 0);
			glVertex3f(size, h+size, 0);
			glColor4f(c1.r,c1.g,c1.b,c1.a);
			glVertex3f(0, h2, 0);
			glVertex3f(0, 0, 0);
		glEnd();
	glPopMatrix();
}
