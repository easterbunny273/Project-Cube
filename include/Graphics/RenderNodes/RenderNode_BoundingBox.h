/*
 * header file for SceneObject_Sphere class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 05/2011, Project FreePool, Project "Bamboo Engine"
 */

#ifndef __bambooengine_sceneobject_boundingbox_header
#define __bambooengine_sceneobject_boundingbox_header

#include "Graphics/RenderNode.h"

//!  A SceneObject which renders a bounding sphere.
/*!
  This SceneObject renders only a red sphere in wireframe mode, used for visualizing the bounding spheres.
*/

class RenderNode_BoundingBox : public RenderNode
{
public:
    /*! \name Constructor / Destructor */
    //@{
    RenderNode_BoundingBox(float fMinX, float fMaxX, float fMinY, float fMaxY, float fMinZ, float fMaxZ);
    ~RenderNode_BoundingBox();
    //@}

protected:
    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender();

	virtual void ItlPreRenderChildren() {};
	virtual void ItlPostRenderChildren() {};
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
