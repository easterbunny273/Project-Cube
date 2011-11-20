#include "RenderNodes/RenderNode_Deferred.h"
#include "RenderNodes/RenderNode_PostEffect.h"
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


    glGenFramebuffers(1, &m_nFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nAlbedoDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_nNormalDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_nTangentDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_nSpecularDrawBuffer, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthDrawBuffer, 0);

    GLenum tDrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, tDrawBuffers);

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
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
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
    glDeleteTextures(1, &m_nDepthDrawBuffer);
}

void Bamboo::RN_Deferred::ItlPreRenderChildren()
{
    //save current viewport params
    glGetIntegerv(GL_VIEWPORT, m_iGeneralViewportParams);

    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    //set viewport size
    glViewport(0,0, m_nWidth, m_nHeight);

    // clear buffers
   // glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Bamboo::RN_Deferred::ItlPostRenderChildren()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //restore viewport params
    glViewport(m_iGeneralViewportParams[0], m_iGeneralViewportParams[1], m_iGeneralViewportParams[2], m_iGeneralViewportParams[3]);
}

void Bamboo::RN_Deferred::ItlPreRender()
{
}

void Bamboo::RN_Deferred::ItlRender()
{
    // just for debugging!

    static Bamboo::RN_PostEffect rPostEffectNode("directwrite");

    GLuint nTextureToShow;

    switch (s_DebugDeferredTexture % 5)
    {
    case 0:
        nTextureToShow = m_nAlbedoDrawBuffer;
        break;
    case 1:
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
    }

    rPostEffectNode.SetTexture("texture1", nTextureToShow );

    rPostEffectNode.Render();


}

void Bamboo::RN_Deferred::ItlPostRender()
{
}

