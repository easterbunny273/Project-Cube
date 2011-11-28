/*
 * header file for RN_Cube class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 11/2011, Project Cube
 */

#ifndef __BAMBOOENGINE_RENDERNODE_SPOTLIGHT
#define __BAMBOOENGINE_RENDERNODE_SPOTLIGHT

#include "RenderNodes/IRenderNode.h"


class Bamboo::RN_SpotLight : public Bamboo::IRenderNode
{
public:
    /*! \name Constructor / Destructor */
    //@{
    RN_SpotLight(glm::vec3 vPosition,
                 glm::vec3 vLookDirection,
                 float fFOV,
                 glm::vec3 vLightColor);

    ~RN_SpotLight();
    //@}
    /*! \name Operations */
    //@{
        //! renders the sceneobject and its children (first, it renders the children).
        /*! First, the itlTestSkipRendering() method is called, if false, itlBeforeRender() gets called.
         *  Then, the render()-method of the children is called and finally the own itlRender()-method is called. */
        //virtual void Render(std::shared_ptr<TItlRenderInfo> pCurrentRenderInfo);
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
    // generates the fbo where the shadomap gets rendered
    void ItlCreateFBO();

    // creates the depth texture
    GLuint ItlCreateDepthTexture();

    /// creates a vbo to draw the spotlight as wireframe
    void ItlCreateVBO();

    void ItlChangeMatrices();
    void ItlRestoreMatrices();
    //@}

    /*! \name Private members */
    //@{
    glm::mat4       m_m4ViewMatrix;
    glm::mat4       m_m4ProjectionMatrix;

    glm::mat4       m_m4SavedProjectionMatrix;
    glm::mat4       m_m4SavedViewMatrix;
    glm::mat4       m_m4SavedViewProjectionMatrix;

    GLuint m_nDepthTexture;
    GLuint m_nFBO;
    int m_iGeneralViewportParams[4];	//viewport params as it were BEFORE we bind our fbo, used to save and restore these values

    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object
    int m_iIndexArraySize;		///< The size of the index array


    //@}
};

#endif
