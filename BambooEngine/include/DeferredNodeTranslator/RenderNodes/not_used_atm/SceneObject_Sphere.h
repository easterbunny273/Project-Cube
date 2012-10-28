/*
 * header file for SceneObject_Sphere class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 10/2010, Project BambooIsland
 * 03/2011, Project FreePool
 */

#ifndef __realtime_sceneobject_sphere_header
#define __realtime_sceneobject_sphere_header

#include "Graphics/SceneObject.h"

//!  A SceneObject which renders a sphere.
/*!
  This SceneObject renders a phong shaded sphere with the given color texture.
*/

//TODO: Clean up source code, we don't need tangents and bitangents anymore.

class SceneObject_Sphere : public SceneObject_BoundingBoxed
{
public:
    /*! \name Constructor / Destructor */
    //@{
    SceneObject_Sphere(float fRadius, const char *szTextureName);
    ~SceneObject_Sphere();
    //@}

    /*! \name SceneObject::IHasVertices interface */
    //@{
	float *GetVertices();
	int *GetIndices();
	int NumIndices();
	int NumVertices();
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

	virtual bool ItlTestSkipRendering();
    //@}

private:
    /*! \name Private members */
    //@{
    float *m_pVertices;
    unsigned int *m_pIndices;

    int m_iNumVertices;
    int m_iNumIndices;

    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object

    const char *m_szTextureName;	///< The texture name (already loaded in TextureManager) used for rendering the sphere.
    //@}
};

#endif
