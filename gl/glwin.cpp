#include "glwin.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
//extern void* glXGetProcAddress(const GLubyte *);
#endif
#include <iostream>
using namespace std;

#ifdef _WIN32
void *getGL(const char *name) {
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) ) {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}
#else
void *getGL(const char *name) {
  void *p = (void *)glXGetProcAddress((GLubyte *)name);
  return p;
}
#endif

PFNGLCREATEPROGRAMPROC    glCreateProgram;
PFNGLDELETEPROGRAMPROC	  glDeleteProgram;
PFNGLCREATESHADERPROC     glCreateShader;
PFNGLSHADERSOURCEPROC     glShaderSource;
PFNGLCOMPILESHADERPROC    glCompileShader;
PFNGLGETSHADERIVPROC      glGetShaderiv;
PFNGLDELETESHADERPROC     glDeleteShader;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLATTACHSHADERPROC 	  glAttachShader;
PFNGLLINKPROGRAMPROC 	  glLinkProgram;
PFNGLGETPROGRAMIVPROC	  glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC		  glUseProgram;
//PFNGLACTIVETEXTUREPROC	  glActiveTexture;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1IPROC		  glUniform1i;
PFNGLUNIFORM4FPROC		  glUniform4f;

void initGL() {
#ifdef _WIN32
  cout << "bazinga: opengl: doing some work Lazy Windows can't do by itself" << endl;
#endif
  cout << "\t" << "loading OpenGL function definitions" << endl;

  glCreateProgram = (PFNGLCREATEPROGRAMPROC)getGL("glCreateProgram");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC)getGL("glDeleteProgram");
  glCreateShader = (PFNGLCREATESHADERPROC)getGL("glCreateShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC)getGL("glShaderSource");
  glCompileShader = (PFNGLCOMPILESHADERPROC)getGL("glCompileShader");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)getGL("glGetShaderiv");
  glDeleteShader = (PFNGLDELETESHADERPROC)getGL("glDeleteShader");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)getGL("glGetShaderInfoLog");
  glAttachShader = (PFNGLATTACHSHADERPROC)getGL("glAttachShader");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)getGL("glLinkProgram");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)getGL("glGetProgramiv");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)getGL("glGetProgramInfoLog");
  glUseProgram = (PFNGLUSEPROGRAMPROC)getGL("glUseProgram");
  //glActiveTexture = (PFNGLACTIVETEXTUREPROC)getGL("glActiveTexture");
  glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)getGL("glGetUniformLocation");
  glUniform1i = (PFNGLUNIFORM1IPROC)getGL("glUniform1i");
  glUniform4f = (PFNGLUNIFORM4FPROC)getGL("glUniform4f");
}
