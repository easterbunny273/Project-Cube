#include "RenderNodes/RenderNode_Deferred.h"
#include "RenderNodes/RenderNode_PostEffect.h"
#include "RenderNodes/RenderNode_SpotLight.h"
#include "TextureManager.h"
#include "Logger.h"

extern int s_DebugDeferredTexture;

Bamboo::RN_Deferred::RN_Deferred(unsigned int nWidth, unsigned int nHeight)
    : m_nWidth(nWidth), m_nHeight(nHeight)
{
    ItlCreateFBO();


}

Bamboo::RN_Deferred::~RN_Deferred()
{
    ItlDeleteFBO();
}


void Bamboo::RN_Deferred::ItlCreateFBO()
{
    TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    //get the id of a free texture unit from the texture manager
    GLuint nUsedTextureUnit = pTextureManager->RequestFreeUnit(); //ask for a free texture unit

    //activate unit
    glActiveTexture(GL_TEXTURE0 + nUsedTextureUnit);

    m_nAlbedoDrawBuffer = ItlCreateColorTexture();
    m_nNormalDrawBuffer = ItlCreateColorTexture();
    m_nTangentDrawBuffer = ItlCreateColorTexture();
    m_nSpecularDrawBuffer = ItlCreateColorTexture();
    m_nDepthDrawBuffer = ItlCreateDepthTexture();
    m_nCombinedDrawBuffer = ItlCreateColorTexture();
    m_nPositionDrawBuffer = ItlCreateColorTexture();


    glGenFramebuffers(1, &m_nFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nAlbedoDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_nNormalDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_nTangentDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_nSpecularDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_nCombinedDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_nPositionDrawBuffer, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthDrawBuffer, 0);

    GLenum tDrawBuffers[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
    glDrawBuffers(6, tDrawBuffers);

    //check fbo status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
        Logger::fatal() << "Failed to initialize FBO for RN_Deferred" << Logger::endl;

    //release used texture unit
    pTextureManager->ReleaseUnit(nUsedTextureUnit);

    //unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


GLuint Bamboo::RN_Deferred::ItlCreateColorTexture()
{
    GLuint nNewTexture;

    //generate color texture (=create new opengl id)
    glGenTextures(1, &nNewTexture);

    //bind color texture
    glBindTexture(GL_TEXTURE_2D, nNewTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    //set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return nNewTexture;
}

GLuint Bamboo::RN_Deferred::ItlCreateDepthTexture()
{
    GLuint nNewTexture;

    //generate depth texture
    glGenTextures(1, &nNewTexture);

    //bind depth texture
    glBindTexture(GL_TEXTURE_2D, nNewTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_nWidth, m_nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


    //set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return nNewTexture;
}

void Bamboo::RN_Deferred::ItlDeleteFBO()
{
    ItlDeleteTextures();

    glDeleteFramebuffers(1, &m_nFBO);
}

void Bamboo::RN_Deferred::ItlDeleteTextures()
{
    glDeleteTextures(1, &m_nAlbedoDrawBuffer);
    glDeleteTextures(1, &m_nNormalDrawBuffer);
    glDeleteTextures(1, &m_nTangentDrawBuffer);
    glDeleteTextures(1, &m_nSpecularDrawBuffer);
    glDeleteTextures(1, &m_nCombinedDrawBuffer);
    glDeleteTextures(1, &m_nDepthDrawBuffer);
    glDeleteTextures(1, &m_nPositionDrawBuffer);
}

void Bamboo::RN_Deferred::ItlPreRenderChildren()
{
    //save current viewport params
    glGetIntegerv(GL_VIEWPORT, m_iGeneralViewportParams);

    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    ItlPushFBO(m_nFBO);

    //set viewport size
    glViewport(0,0, m_nWidth, m_nHeight);

    // clear buffers
   // glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Bamboo::RN_Deferred::ItlPostRenderChildren()
{
    //remove the fbo (THIS fbo) from the bound_fbos stack
    ItlPopFBO();

    //if there was a SceneObject_FBO bound in the SceneObject_Tree, rebind the previous bound fbo
    if (ItlIsNestedFBO())
    {
        GLuint previous_bound = ItlGetTopFBO();
        glBindFramebuffer(GL_FRAMEBUFFER, previous_bound);
    }
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //restore viewport params
    glViewport(m_iGeneralViewportParams[0], m_iGeneralViewportParams[1], m_iGeneralViewportParams[2], m_iGeneralViewportParams[3]);
}

void Bamboo::RN_Deferred::ItlPreRender()
{
}

void Bamboo::RN_Deferred::ItlRender()
{
    glGetIntegerv(GL_VIEWPORT, m_iGeneralViewportParams);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);
    ItlPushFBO(m_nFBO);
    glViewport(0,0, m_nWidth, m_nHeight);
    GLenum tDrawBuffers[1] = { GL_COLOR_ATTACHMENT4 };
    glDrawBuffers(1, tDrawBuffers);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);



    // first, we search for all lights
    for (unsigned int a=0; a < m_vspSpotLights.size(); a++)
    {
        std::shared_ptr<Bamboo::RN_SpotLight> spLight (m_vspSpotLights[a]);
        spLight->SetTextureLocation("color_texture", m_nAlbedoDrawBuffer);
        spLight->SetTextureLocation("normal_texture", m_nNormalDrawBuffer);
        spLight->SetTextureLocation("depth_texture", m_nDepthDrawBuffer);
        spLight->SetTextureLocation("position_texture", m_nPositionDrawBuffer);

        TItlRenderInfo *pCurrentRenderInfo = m_pCurrentRenderInfo.get();

        //store old matrices
        glm::mat4 SavedModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix;
        glm::mat4 SavedInverseModelViewProjectionMatrix = pCurrentRenderInfo->InverseModelViewProjectionMatrix;
        glm::mat4 SavedModelMatrix = pCurrentRenderInfo->ModelMatrix;
        glm::mat4 SavedModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling;

        //update matrices by multiplying with our matrices
        pCurrentRenderInfo->ModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix * m_mTransformMatrixToWorld;
        pCurrentRenderInfo->InverseModelViewProjectionMatrix = pCurrentRenderInfo->InverseModelViewProjectionMatrix * m_mTransformMatrixFromWorld;
        pCurrentRenderInfo->ModelMatrix = pCurrentRenderInfo->ModelMatrix * m_mTransformMatrixToWorld;
        pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling * m_mTransformMatrixToWorld;

        spLight->Render(this);

        //write old matrices
        pCurrentRenderInfo->ModelViewProjectionMatrix = SavedModelViewProjectionMatrix;
        pCurrentRenderInfo->InverseModelViewProjectionMatrix = SavedInverseModelViewProjectionMatrix;
        pCurrentRenderInfo->ModelMatrix = SavedModelMatrix;
        pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = SavedModelViewProjectionMatrix_ForFrustumCulling;
    }

    GLenum tDrawBuffers2[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 , GL_COLOR_ATTACHMENT5};
    glDrawBuffers(6, tDrawBuffers2);


    ItlPostRenderChildren();

    // ---BEGIN ---- this is just for debugging!

    static Bamboo::RN_PostEffect rPostEffectNode("directwrite");

    GLuint nTextureToShow;

    switch (s_DebugDeferredTexture % 2)
    {
    case 0:
        nTextureToShow = m_nAlbedoDrawBuffer;
        break;
    case 1:
        nTextureToShow = m_nCombinedDrawBuffer;
        break;
    /*case 1:
        nTextureToShow = m_nNormalDrawBuffer;
        break;
    case 2:
        nTextureToShow = m_nTangentDrawBuffer;
        break;
    case 3:
        nTextureToShow = m_nSpecularDrawBuffer;
        break;
    case 4:
        nTextureToShow = m_nDepthDrawBuffer;
        break;
    case 5:
        nTextureToShow = m_nPositionDrawBuffer;
        break;
    case 6:
        nTextureToShow = m_nCombinedDrawBuffer;
        break;*/
    }

    rPostEffectNode.SetTexture("texture1", nTextureToShow );

    rPostEffectNode.Render();


}

void Bamboo::RN_Deferred::ItlPostRender()
{
}

void Bamboo::RN_Deferred::AddSpotLight(std::shared_ptr<Bamboo::RN_SpotLight> spSpotlight)
{
    m_vspSpotLights.push_back(spSpotlight);
}

