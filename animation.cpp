#include "image.h"
#include "animation.h"
#include "filesystem.h"
#include "json.h"
#include "cache.h"
#include "bazinga.h"
#include <SDL_opengl.h>
using namespace bazinga;

Animation::Animation (Path fpath) {
  char *data = fs::getFileData(fpath);
  string sData = string (data);
  BjObject *jObject = json::parse (sData);
  
  // Load the data in jObject
  auto imgValue = jObject->get("file");

  if (imgValue->type == BjValue::jString) {
    img = Path(imgValue->str);
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'file'" << endl;
    cout << "\tmust be String" << endl;
  }

  auto wValue = jObject->get("width");

  if (wValue->type == BjValue::jNumber) {
    w = wValue->number;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'width'" << endl;
    cout << "\tmust be Number" << endl;
  }

  auto hValue = jObject->get("height");

  if (hValue->type == BjValue::jNumber) {
    h = hValue->number;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'height'" << endl;
    cout << "\tmust be Number" << endl;
  }

  auto dirValue = jObject->get("direction");

  if (dirValue->type == BjValue::jString &&
      (dirValue->str == "forward" ||
      dirValue->str == "backward")) {
    direction = dirValue->str == "forward"?1:-1;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'direction'" << endl;
    cout << "\tmust be String ('forward' or 'backward')" << endl;
  }

  auto typeValue = jObject->get("type");

  if (typeValue->type == BjValue::jString &&
      (typeValue->str == "horizontal" ||
      typeValue->str == "vertical")) {
    type = typeValue->str == "horizontal"?Horizontal:Vertical;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'type'" << endl;
    cout << "\tmust be String ('horizontal' or 'vertical')" << endl;
  }

  auto durValue = jObject->get("duration");

  if (durValue->type == BjValue::jNumber) {
    duration = durValue->number;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'duration'" << endl;
    cout << "\tmust be Number" << endl;
  }

  auto loopValue = jObject->get("loop");

  if (loopValue->type == BjValue::jTrue ||
      loopValue->type == BjValue::jFalse) {
    loop = loopValue->type == BjValue::jTrue;
  } else {
    cout << "bazinga: anim: incorrect type for paramater 'loop'" << endl;
    cout << "\tmust be true or false" << endl;
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
  		frame = image->w/w-1;
  	}
  	if (frame >= image->w/w) {
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