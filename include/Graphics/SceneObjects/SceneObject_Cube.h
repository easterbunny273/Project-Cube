/*
 * header file for SceneObject_Cube class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
               Florian Spechtenhauser <florian.spechtenhauser@gmail.com>
 * 07/2011, Project Cube
 */

#ifndef __bambooengine_sceneobject_boundingbox_header
#define __bambooengine_sceneobject_boundingbox_header

#include "Graphics/SceneObject.h"

//!  A SceneObject which renders a magic cube (from the inside)
/*!
  This SceneObject renders a cube from the inside.
*/

class SceneObject_Cube : public SceneObject
{
public:
    /*! \name Constructor / Destructor */
    //@{
    SceneObject_Cube();
    ~SceneObject_Cube();
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

    /*! \name other helper functions */
    //@{
	virtual void ItlLoadRessources();
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
