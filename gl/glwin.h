#ifndef BAZINGA_GLWIN_H
#define BAZINGA_GLWIN_H

// Ohhh, BEAUTIFUL, windows DO NOT
// define MOST opengl functions.
// Truly wonderful
#ifdef _WIN32
#undef GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>

extern PFNGLCREATEPROGRAMPROC    glCreateProgram;
extern PFNGLCREATESHADERPROC     glCreateShader;
extern PFNGLSHADERSOURCEPROC     glShaderSource;
extern PFNGLCOMPILESHADERPROC    glCompileShader;
extern PFNGLGETSHADERIVPROC      glGetShaderiv;
extern PFNGLDELETESHADERPROC     glDeleteShader;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC     glAttachShader;
extern PFNGLDELETEPROGRAMPROC    glDeleteProgram;
extern PFNGLLINKPROGRAMPROC      glLinkProgram;
extern PFNGLGETPROGRAMIVPROC     glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC       glUseProgram;
extern PFNGLACTIVETEXTUREPROC    glActiveTexture;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC		 glUniform1i;
extern PFNGLUNIFORM4FPROC		 glUniform4f;

#endif /* _WIN32 */

void initGL ();

#endif /* BAZINGA_GLWIN_H */