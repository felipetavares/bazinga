#include "video.h"
#include <cmath>
#include <il.h>
using namespace bazinga;

video::Image::Image (Path path) {
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilLoadImage(path.getPath().c_str());
	void *data = ilGetData();
	if(!data) {
		ilBindImage(0);
		ilDeleteImages(1, &imageID);
		return;
	}

	int const width  = ilGetInteger(IL_IMAGE_WIDTH);
	int const height = ilGetInteger(IL_IMAGE_HEIGHT);
	int const depth  = ilGetInteger(IL_IMAGE_DEPTH);
    int const bpp    = ilGetInteger(IL_IMAGE_BPP);
	int const type   = ilGetInteger(IL_IMAGE_TYPE); // matches OpenGL
	int const format = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL

    int nw = pow(2, ceil(log2(width)));
    int nh = pow(2, ceil(log2(height)));

    ILuint potImageID;
    ilGenImages (1, &potImageID);
    ilBindImage (potImageID);
    
    ilTexImage (nw, nh, depth, bpp, format, type, NULL);

    ilBlit (imageID, 0, 0, 0, 0, 0, 0, width, height, depth);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packed

    glTexImage2D(GL_TEXTURE_2D, 0, format, nw, nh, 0, format, type, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    cout << "bazinga: image: image loadded sucessfully (" << nw << "," << nh << ")" << endl;

    ilSaveImage("debug.bmp");

    id = textureID;
    w = width;
    h = height;
    rw = nw;
    rh = nh;
}

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
		cout << "[ERR] [CANNOT INIT SDL]" << endl;
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
	cout << "[INF] Loading icon from " << icon.getPath() << endl;
	// TODO: add SDL_image
	//SDL_WM_SetIcon(IMG_Load(icon.getPath().c_str()), NULL);
	screen = SDL_SetVideoMode (windowWidth,windowHeight,windowBpp,videoFlags);
	SDL_ShowCursor (0);

	cout << "[CRT] [WIN] [w:" << windowWidth << ",h:" << windowHeight << "]" << endl;

	glEnable (GL_DEPTH_TEST);

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA );

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	cout << "[INF]" << " Using OpenGL version: " << glGetString(GL_VERSION) << endl;
}

void video::deinit () {
	delete videoModes;
}

void video::getVideoModes () {
	SDL_Rect **modeList;

	cout << "[INF] [QUERYING VIDEO MODES]" << endl;

	modeList = SDL_ListModes (NULL, videoFlags);

	cout << "[INF] [GOT VIDEO MODES FROM SDL]" << endl;

	if (modeList != NULL) {
		if (modeList == (SDL_Rect**)-1) {
			cout << "[INF] [ALL RESOLUTIONS AVAILABLE]" << endl;
			anyResolution = true;
		} else {
			for (int i=0;modeList[i];i++) {
				videoModes->push_back (*modeList[i]);
			}
		}
	} else {
		cout << "[ERR] [NO VIDEO MODE AVAIABLE]" << endl;
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

void video::renderMap (Map *map) {
	glClearColor (0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	map->render();
}

void video::render () {
	SDL_GL_SwapBuffers();
}
