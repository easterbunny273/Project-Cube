#ifndef __COMMON_OPENGL_HEADER
#define __COMMON_OPENGL_HEADER

//include extension wrapper - important: necessary to include BEFORE glfw to prevent compile errors
#include <GL/glew.h>

//include glfw
#define GLFW_NO_GLU
#include <GL/glfw.h>

//to detect unallowed use of old opengl-functions (functions which are NOT in the core profile), we had an own header which redefines the function calls to emit compile errors
#include "prevent_old_opengl.h"

//helper function for translating glerrors to char*
inline const char *translateGLerror(GLenum error)
{
    switch (error)
    {
    case GL_INVALID_ENUM:
	return "GL_INVALID_ENUM";
	break;
    case GL_INVALID_VALUE:
	return "GL_INVALID_VALUE";
	break;
    case GL_INVALID_OPERATION:
	return "GL_INVALID_OPERATION";
	break;
    case GL_STACK_OVERFLOW:
	return "GL_STACK_OVERFLOW";
	break;
    case GL_STACK_UNDERFLOW:
	return "GL_STACK_UNDERFLOW";
	break;
    case GL_OUT_OF_MEMORY:
	return "GL_OUT_OF_MEMORY";
	break;
    case GL_TABLE_TOO_LARGE:
	return "GL_TABLE_TOO_LARGE";
	break;
    default:
	return "unknown GL error";
	break;
    }
};

#endif
