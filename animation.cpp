#include "image.h"
#include "animation.h"
#include "filesystem.h"
#include "json.h"
#include "cache.h"
#include "bazinga.h"
#include "console.h"
#include <SDL_opengl.h>
using namespace bazinga;

Animation::Animation (Path fpath):
  ended(false) {
  char *data = fs::getFileData(fpath);
  string sData = string (data);
  BjObject *jObject = json::parse (sData);

  // Load the data in jObject
  auto imgValue = jObject->get("file");

  if (imgValue->type == BjValue::jString) {
    img = Path(imgValue->str);
  } else {
    console << "bazinga: anim: incorrect type for paramater 'file'" << outline;
    console << "\tmust be String" << outline;
  }

  auto wValue = jObject->get("width");

  if (wValue->type == BjValue::jNumber) {
    w = wValue->number;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'width'" << outline;
    console << "\tmust be Number" << outline;
  }

  auto hValue = jObject->get("height");

  if (hValue->type == BjValue::jNumber) {
    h = hValue->number;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'height'" << outline;
    console << "\tmust be Number" << outline;
  }

  auto dirValue = jObject->get("direction");

  if (dirValue->type == BjValue::jString &&
      (dirValue->str == "forward" ||
      dirValue->str == "backward")) {
    direction = dirValue->str == "forward"?1:-1;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'direction'" << outline;
    console << "\tmust be String ('forward' or 'backward')" << outline;
  }

  auto typeValue = jObject->get("type");

  if (typeValue->type == BjValue::jString &&
      (typeValue->str == "horizontal" ||
      typeValue->str == "vertical")) {
    type = typeValue->str == "horizontal"?Horizontal:Vertical;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'type'" << outline;
    console << "\tmust be String ('horizontal' or 'vertical')" << outline;
  }

  auto durValue = jObject->get("duration");

  if (durValue->type == BjValue::jNumber) {
    duration = durValue->number;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'duration'" << outline;
    console << "\tmust be Number" << outline;
  }

  auto loopValue = jObject->get("loop");

  if (loopValue->type == BjValue::jTrue ||
      loopValue->type == BjValue::jFalse) {
    loop = loopValue->type == BjValue::jTrue;
  } else {
    console << "bazinga: anim: incorrect type for paramater 'loop'" << outline;
    console << "\tmust be true or false" << outline;
  }

  delete jObject;
  delete data;

  frame = 0;
  nextFrame = bazinga::curtime+duration;
}

void Animation::render () {
  Image *image = cache::getTexture(img);
  GLuint texID = image->id;
  float sX, eX, sY, eY;

  if (type == Horizontal) {
	  sX = float(frame*w)/((float)image->rw);
	  sY = float(0)/((float)image->rh);
	  eX = float((frame+1)*w)/((float)image->rw);
	  eY = float(image->h)/((float)image->rh);
	} else {
	  sX = float(0)/((float)image->rw);
	  sY = float(frame*h)/((float)image->rh);
	  eX = float(image->w)/((float)image->rw);
	  eY = float((frame+1)*h)/((float)image->rh);
	}

  glPushMatrix();
    //glTranslatef (0, 0, 0);
    glEnable (GL_TEXTURE);
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texID);
    glBegin(GL_QUADS);
      glTexCoord2f (sX,1-sY);  glVertex3f(-w/2, -h/2, 0);
      glTexCoord2f (eX,1-sY);  glVertex3f(w/2, -h/2, 0);
      glTexCoord2f (eX,1-eY);  glVertex3f(w/2, h/2, 0);
      glTexCoord2f (sX,1-eY);  glVertex3f(-w/2, h/2, 0);
    glEnd();
  glPopMatrix();

  if (bazinga::curtime > nextFrame) {
  	nextFrame = bazinga::curtime+duration;
  	frame += direction;

  	if (frame < 0) {
      ended = true;
    	frame = image->w/w-1;
  	}
  	if (frame >= image->w/w) {
  		ended = true;
      frame = 0;
  	}
  }
}

int Animation::getWidth() {
  return w;
}

int Animation::getHeight() {
  return h;
}

bool Animation::isEnded () {
  return ended;
}
