/*
 * header file for SceneObject_VolumeVisRaycaster class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 05/2011 Project "BambooEngine"
 */

#ifndef __bambooengine_sceneobject_volumevisraycaster_header
#define __bambooengine_sceneobject_volumevisraycaster_header

#include "Graphics/SceneObject.h"

/**
  * TODO: comment
*/

class SceneObject_VolumeVisRaycaster : public SceneObject
{
public:
    /*! \name Access to Vertices / Indices */
    //@{
	SceneObject_VolumeVisRaycaster();
    //@}

    /*! \name Access to Vertices / Indices */
    //@{
	virtual float *GetVertices() { return NULL; };
	virtual int *GetIndices() { return NULL; };
	virtual int NumIndices() { return 0; };
	virtual int NumVertices() { return 0; };
    //@}


protected:
    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the render() method calls the render() methods of the children,
	    and can be used to bind a fbo (to render the children in this fbo) or something like that */
	virtual void ItlPreRenderChildren() {};

	/*! this method is called after all children were rendered,
	    and can be used to unbind a fbo or something like that */
	virtual void ItlPostRenderChildren() {};

	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender() {};

	virtual bool ItlTestSkipRendering();
    //@}

private:
    /*! \name Private members */
    //@{
    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object
    int m_iIndexArraySize;		///< The size of the index array

    //@}
};

#endif
