/*
 * header file for SceneObject_LightSphere class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 01/2011
 */

#ifndef __realtime_sceneobject_lightsphere_header
#define __realtime_sceneobject_lightsphere_header

#include "Graphics/SceneObject.h"
#include "Graphics/SceneObjects/SceneObject_EmptyNode.h"

//!  A SceneObject which draws a white sphere, which should represent the sun.
/*!

*/

class SceneObject_LightSphere : public SceneObject
{
private:
     GLuint vertex_buffer;
     GLuint index_buffer;
     int indexArray_size;
     GLuint vao;

protected:
    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the render() method calls the render() methods of the children,
	    and can be used to bind a fbo (to render the children in this fbo) or something like that */
	virtual void ItlPreRenderChildren();

	/*! this method is called after all children were rendered,
	    and can be used to unbind a fbo or something like that */
	virtual void ItlPostRenderChildren();

	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender();
    //@}

public:

    SceneObject_LightSphere();
    ~SceneObject_LightSphere();
};

#endif
