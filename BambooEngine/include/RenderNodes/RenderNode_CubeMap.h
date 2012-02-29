/*
 * header file for RN_CubeMap class
 * written by: Christian Möllinger <ch.moellinger@gmail.com>
 * 01/2012, Project Cube
 */

#ifndef __BAMBOOENGINE_RENDERNODE_CUBEMAP
#define __BAMBOOENGINE_RENDERNODE_CUBEMAP

#include "RenderNodes/IRenderNode.h"


class Bamboo::RN_CubeMap : public Bamboo::IRenderNode
{
public:
    /*! \name Constructor / Destructor */
    //@{
    RN_CubeMap  (glm::vec3 vPosition, GLuint nResolution = 512);

    ~RN_CubeMap();
    //@}
protected:
    /*! \name SceneObject Interface */
    //@{
        /*! this method is called before the sceneobject itself gets rendered.
            shaders and things like that should be activated in this method */
        virtual void ItlPreRender() {};

        /*! this method is called to render the sceneobject.
            Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
            the transform matrices must be sent again */
        virtual void ItlRender() {};

        /*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
        virtual void ItlPostRender() {};

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
    void ItlCreateFBO();
    //@}

    /*! \name Private members */
    //@{
      glm::vec3 m_v3Position;

      GLuint    m_nCubeColorTexture;
      //GLuint    m_nDepthBuffer;
      GLuint    m_nCubeDepthTexture;
      GLuint    m_nResolution;

      GLuint    m_nFBO;
    //@}
};

#endif
