#ifndef BAZINGA_GLWIN_H
#define BAZINGA_GLWIN_H

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
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC		 glUniform1i;
extern PFNGLUNIFORM4FPROC		 glUniform4f;

#ifdef _WIN32
extern PFNGLACTIVETEXTUREPROC    glActiveTexture;
#endif

void initGL ();

#endif /* BAZINGA_GLWIN_H */
