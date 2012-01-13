/*
 * header file for RN_Cube class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 11/2011, Project Cube
 */

#ifndef __BAMBOOENGINE_RENDERNODE_SPOTLIGHTMODEL
#define __BAMBOOENGINE_RENDERNODE_SPOTLIGHTMODEL

#include "RenderNodes/IRenderNode.h"


class Bamboo::RN_SpotLight_Model : public Bamboo::IRenderNode
{
public:
    /*! \name Constructor / Destructor */
    //@{
    RN_SpotLight_Model(glm::vec3 vPosition,
                 glm::vec3 vLookDirection,
                 float fFOV,
                 glm::vec3 vLightColor);

    ~RN_SpotLight_Model();
    //@}
    /*! \name Operations */
    //@{
        void SetTextureLocation(std::string sTexture, GLint iLocation);
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

        virtual void ItlPreRenderChildren();
        virtual void ItlPostRenderChildren();
    //@}

    /*! \name other helper functions */
    //@{
        virtual void ItlLoadRessources();
    //@}
private:
    /*! \name Internal help methods */
    //@{
    /// creates a vbo to draw the spotlight as wireframe
    void ItlCreateVBO();

    /// prepares the vertex array object
    void ItlPrepareVAO();
    //@}

    /*! \name Private members */
    //@{
    glm::mat4       m_m4ViewMatrix;
    glm::mat4       m_m4ProjectionMatrix;

    glm::vec3       m_vLightColor;

    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object
    int m_iIndexArraySize;		///< The size of the index array
    //@}
};

#endif
