#include "image.h"
#include "filesystem.h"
#include <sstream>
#include <il.h>
using namespace bazinga;

Image::Image (Path path) {
  ILuint imageID;
  ilGenImages(1, &imageID);
  ilBindImage(imageID);

  ilLoadImage(path.getPath().c_str());

  int const width  = ilGetInteger(IL_IMAGE_WIDTH);
  int const height = ilGetInteger(IL_IMAGE_HEIGHT);
  int const depth  = ilGetInteger(IL_IMAGE_DEPTH);
  int const bpp    = ilGetInteger(IL_IMAGE_BPP);
  int const type   = ilGetInteger(IL_IMAGE_TYPE); 	// matches OpenGL
  int const format = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL

  int nw = pow(2, ceil(log2(width)));
  int nh = pow(2, ceil(log2(height)));

  ILuint potImageID;
  ilGenImages (1, &potImageID);
  ilBindImage (potImageID);

  ilTexImage (nw, nh, depth, bpp, format, type, NULL);

	void *data = ilGetData();
	if(!data) {
		ilBindImage(0);
		ilDeleteImages(1, &potImageID);
		return;
	}

	ilDisable(IL_BLIT_BLEND);

  ilOverlayImage (imageID, 0, 0, 0);

	int const ptype   = ilGetInteger(IL_IMAGE_TYPE); 	// matches OpenGL
	int const pformat = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL

	GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packed

	glTexImage2D(GL_TEXTURE_2D, 0, pformat, nw, nh, 0, pformat, ptype, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  cout << "bazinga: image: image loaded sucessfully (" << nw << "," << nh << ")" << endl;

	// The following is debugging code
	/*
	stringstream ss;

	ss << "debug" << potImageID << ".bmp";

	ilSaveImage(ss.str().c_str());
	*/

  id = textureID;
  w = width;
  h = height;
  rw = nw;
  rh = nh;

  ilDeleteImages (1, &imageID);
  ilDeleteImages (1, &potImageID);
}

Image::~Image () {
  glDeleteTextures(1, &id);
}