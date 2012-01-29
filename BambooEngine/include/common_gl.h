/*
 * header file for Logger class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * written by: florian spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

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
inline const char *TranslateGLerror(GLenum error)
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
}


//helper function for translating glerrors to char*
inline const char *TranslateFBOStatus(GLenum status)
{
    switch (status)
    {
    case GL_FRAMEBUFFER_UNDEFINED:
        return "GL_FRAMEBUFFER_UNDEFINED";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
        break;
    default:
        return "unknown fbo status";
        break;
    }
}
#endif
