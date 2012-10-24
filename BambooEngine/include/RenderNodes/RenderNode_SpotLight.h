/*
 * header file for RN_Cube class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 11/2011, Project Cube
 */

#ifndef __BAMBOOENGINE_RENDERNODE_SPOTLIGHT
#define __BAMBOOENGINE_RENDERNODE_SPOTLIGHT

#include "RenderNodes/IRenderNode.h"

namespace BambooGraphics
{
class GraphicsCore::RN_SpotLight : public GraphicsCore::IRenderNode
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
    // generates the fbo where the shadomap gets rendered
    void ItlCreateFBO();

    // creates the depth texture
    GLuint ItlCreateDepthTexture();

    GLuint ItlCreateColorTexture();

    /// creates a vbo to draw the spotlight as wireframe
    void ItlCreateVBO();

    /// prepares the vertex array object
    void ItlPrepareVAO();

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

    glm::vec3       m_vLightColor;
    glm::vec3       m_vLightPosition;


    glm::mat4       m_m4LastUsedViewProjectionMatrix;
    glm::vec3       m_v3LastUsedLightPosition;

    GLuint m_nColorTexture;
    GLuint m_nDepthTexture;
    GLuint m_nFBO;
    int m_iGeneralViewportParams[4];	//viewport params as it were BEFORE we bind our fbo, used to save and restore these values

    GLuint m_nVertexArrayObject;	///< The opengl name (=unsigned int) of the vertex array object
    GLuint m_nVertexBufferObject;	///< The opengl name (=unsigned int) of the vertex buffer object
    GLuint m_nIndexBufferObject;	///< The opengl name (=unsigned int) of the index buffer object
    int m_iIndexArraySize;		///< The size of the index array

    std::map<std::string, GLint>   m_mTextureLocations;

    //@}
};


}
#endif

