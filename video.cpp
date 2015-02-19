#include "video.h"
#include "bazinga.h"
#include "console.h"
#include <cmath>
#include <il.h>
using namespace bazinga;

float video::cr = 0, video::cg = 0, video::cb = 0, video::ca = 0;
float video::bcr = 0, video::bcg = 0, video::bcb = 0;
float video::fcr = 0, video::fcg = 0, video::fcb = 0;
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

video::video () {
	endtime = -1;
	starttime = -1;
}

video::~video () {
}

void video::init() {
	if (SDL_Init (SDL_INIT_EVERYTHING) < 0) {
		console << LINEINFO << "cannot Init SDL" << outline;
		exit (-1);
	}

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
	//SDL_GL_SetAttribute(SDL_GL_SAMPLE_BUFFERS_ARB, 1);
	//SDL_GL_SetAttribute(SDL_GL_SAMPLES_ARB, 2);

	anyResolution = false;
	// Initialize video modes array
	videoModes = new vector <SDL_Rect>();
	getVideoModes();

	findBestVideoMode();

	screen = SDL_SetVideoMode (windowWidth,windowHeight,windowBpp,videoFlags);
	SDL_ShowCursor (1);

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
		windowWidth = 800;
		windowHeight = 600;
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
	SDL_WM_SetIcon(SDL_LoadBMP(icon.getPath().c_str()), NULL);
}

void video::renderMap (Map *map) {
	glClearColor (bcr,bcg,bcb,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	map->render();
}

void video::setColor (float r, float g, float b, float a) {
	cr = r;
	cg = g;
	cb = b;
	ca = a;
}

void video::setBackgroundColor (float r, float g, float b) {
	bcr = r;
	bcg = g;
	bcb = b;
}

void video::fillRect (int x, int y, int w, int h) {
	glPushMatrix();
		glColor4f(cr,cg,cb,ca);
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
	glColor4f(cr,cg,cb,ca);
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
		glColor4f(cr,cg,cb,ca);
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
	glColor4f(cr,cg,cb,ca);
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

		video::setColor(fcr,fcg,fcb, factor);
		video::fillRect(-windowWidth/2, -windowHeight/2, windowWidth, windowHeight);
	}

	SDL_GL_SwapBuffers();
}

void video::fadeFrom (float r, float g, float b, float time) {
	fcr = r;
	fcg = g;
	fcb = b;

	endtime = bazinga::curtime+time;
	starttime = bazinga::curtime;
	fadedirection = true;
}

void video::fadeTo (float r, float g, float b, float time) {
	fcr = r;
	fcg = g;
	fcb = b;

	endtime = bazinga::curtime+time;
	starttime = bazinga::curtime;
	fadedirection = false;
}