/*
 * header file for Bamboo::RN_Deferred class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOO_RENDERNODE_DEFERRED_HEADER
#define __BAMBOO_RENDERNODE_DEFFERED_HEADER

#include "RenderNodes/IRenderNode.h"
#include "RenderNodes/RenderNode_SpotLight.h"

#include <vector>
#include <memory>


//!  A RenderNode which does deferred lighting.
/*!
    This RenderNode is not a real SceneObject as an object in the scene,
    rather it's an object which can be used in the sceneobjects-tree to render all
    children deferred into multiple render targets (albedo, normals, specular, ...)
    and does the lighting based on these created textures
*/

class Bamboo::RN_Deferred : public Bamboo::IRenderNode
{
public:
    /*! \name Constructors / Destructor */
    //@{

    /// constructor
    RN_Deferred(unsigned int nWidth, unsigned int nHeight, bool bLayered = false);

    /// destructor
    virtual ~RN_Deferred();

    //@}

    /*! \name xy */
    //@{
        void AddSpotLight(std::shared_ptr<Bamboo::RN_SpotLight> spSpotlight);

        GLuint GetAlbedoTexture() { return m_nAlbedoDrawBuffer; }
    //@}

    /*! \name Bamboo::IRenderNode interface */
    //@{
        void ItlPreRenderChildren();

        void ItlPostRenderChildren();

        void ItlPreRender();

        void ItlRender();

        void ItlPostRender();
    //@}
private:
    /*! \name Internal helper methods */
    //@{
    /// creates the fbo with the multiple color buffers
    void ItlCreateFBO();
    void ItlCreateLayeredFBO();

    /// cleaning up the fbo
    void ItlDeleteFBO();

    /// creates an opengl color texture with the parameters needed for this node
    GLuint ItlCreateColorTexture();
    GLuint ItlCreateLayeredColorTexture();

    /// creates an opengl depth texture with the parameters needed for this node
    GLuint ItlCreateDepthTexture();
    GLuint ItlCreateLayeredDepthTexture();

    void    ItlDeleteTextures();
    //@}

    /*! \name Private members */
    //@{
        unsigned int m_nWidth;
        unsigned int m_nHeight;

        GLuint      m_nAlbedoDrawBuffer;
        GLuint      m_nStencilDrawBuffer;
        GLuint      m_nNormalDrawBuffer;
        GLuint      m_nNormalMapDrawBuffer;
        GLuint      m_nTangentDrawBuffer;
        GLuint      m_nSpecularDrawBuffer;
        GLuint      m_nPositionDrawBuffer;
        GLuint      m_nDepthDrawBuffer;

        GLuint      m_nDisplaceDrawBuffer;

        GLuint      m_nFBO;


        bool    m_bLayeredFBO;

        std::vector<std::shared_ptr<Bamboo::RN_SpotLight> > m_vspSpotLights;
    //@}
};

#endif
