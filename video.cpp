#include "video.h"
#include <cmath>
#include <il.h>
using namespace bazinga;

float video::cr = 0, video::cg = 0, video::cb = 0, video::ca = 0;
float video::bcr = 0, video::bcg = 0, video::bcb = 0;
int video::windowBpp = 0;
int video::windowWidth = 0;
int video::windowHeight = 0;
bool video::anyResolution = false;
vector <SDL_Rect> *video::videoModes = NULL;
Path video::icon;
SDL_Surface *video::screen = NULL;

video::video () {
}

video::~video () {
}

void video::init() {
	if (SDL_Init (SDL_INIT_EVERYTHING) < 0) {
		cout << "bazinga: video: cannot init SDL" << endl;
		exit (-1);
	}

	ilInit();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	anyResolution = false;
	// Initialize video modes array
	videoModes = new vector <SDL_Rect>();
	getVideoModes();

	findBestVideoMode();

	icon = Path (".:Assets:BazingaEngineLittle.png");
	cout << "bazinga: video: loading icon from " << icon.getPath() << endl;
	// TODO: add SDL_image
	//SDL_WM_SetIcon(IMG_Load(icon.getPath().c_str()), NULL);
	screen = SDL_SetVideoMode (windowWidth,windowHeight,windowBpp,videoFlags);
	SDL_ShowCursor (1);

	cout << "bazinga: video: window (" << windowWidth << ", " << windowHeight << ")" << endl;

  // CHANGE THAT IF NEED BE
	glDisable (GL_DEPTH_TEST);

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA );

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

	cout << "bazinga: video: OpenGL: " << glGetString(GL_VERSION) << endl;
}

void video::deinit () {
	delete videoModes;
}

void video::getVideoModes () {
	SDL_Rect **modeList;

	cout << "bazinga: video: querying video modes" << endl;

	modeList = SDL_ListModes (NULL, videoFlags);

	cout << "bazinga: video: loaded video modes from SDL" << endl;

	if (modeList != NULL) {
		if (modeList == (SDL_Rect**)-1) {
			cout << "bazinga: video: all resolutions available" << endl;
			anyResolution = true;
		} else {
			for (int i=0;modeList[i];i++) {
				videoModes->push_back (*modeList[i]);
			}
		}
	} else {
		cout << "bazinga: video: no video modes found" << endl;
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

void video::render () {
	SDL_GL_SwapBuffers();
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