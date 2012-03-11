#include "RenderNodes/RenderNode_Deferred.h"
#include "RenderNodes/RenderNode_PostEffect.h"
#include "RenderNodes/RenderNode_SpotLight.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "PC_Logger.h"

extern int s_DebugDeferredTexture;
extern int s_nUseParallax;

Bamboo::RN_Deferred::RN_Deferred(unsigned int nWidth, unsigned int nHeight, bool bLayered /* = FALSE */)
    : m_nWidth(nWidth), m_nHeight(nHeight)
{
  if (bLayered)
    ItlCreateLayeredFBO();
  else
    ItlCreateFBO();

    m_bLayeredFBO = bLayered;

    Logger::debug() << "RN_Deferred created" << Logger::endl;
}

Bamboo::RN_Deferred::~RN_Deferred()
{
    ItlDeleteFBO();

    Logger::debug() << "RN_Deferred destroyed" << Logger::endl;
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
    m_nNormalMapDrawBuffer = ItlCreateColorTexture();
    m_nNormalDrawBuffer = ItlCreateColorTexture();
    m_nTangentDrawBuffer = ItlCreateColorTexture();
    m_nSpecularDrawBuffer = ItlCreateColorTexture();
    m_nDepthDrawBuffer = ItlCreateDepthTexture();
    m_nDisplaceDrawBuffer = ItlCreateColorTexture();
    m_nPositionDrawBuffer = ItlCreateColorTexture();
    m_nStencilDrawBuffer = ItlCreateColorTexture();

    glGenFramebuffers(1, &m_nFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nAlbedoDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_nNormalDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_nTangentDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_nSpecularDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_nDisplaceDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_nPositionDrawBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_nNormalMapDrawBuffer, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_nDepthDrawBuffer, 0);

    GLenum tDrawBuffers[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
    glDrawBuffers(7, tDrawBuffers);

    //check fbo status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
        Logger::fatal() << "Failed to initialize FBO for RN_Deferred" << Logger::endl;

    //release used texture unit
    pTextureManager->ReleaseUnit(nUsedTextureUnit);

    //unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_bLayeredFBO = false;
}

void Bamboo::RN_Deferred::ItlCreateLayeredFBO()
{
  TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
  assert (pTextureManager != NULL);

  GLenum error = glGetError();
  assert (error == GL_NO_ERROR);

  //get the id of a free texture unit from the texture manager
  GLuint nUsedTextureUnit = pTextureManager->RequestFreeUnit(); //ask for a free texture unit

  //activate unit
  glActiveTexture(GL_TEXTURE0 + nUsedTextureUnit);

  error = glGetError();
  assert (error == GL_NO_ERROR);

  m_nAlbedoDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nNormalMapDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nNormalDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nTangentDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nSpecularDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nDepthDrawBuffer = ItlCreateLayeredDepthTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nDisplaceDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nPositionDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);
  m_nStencilDrawBuffer = ItlCreateLayeredColorTexture();
  error = glGetError();
  assert (error == GL_NO_ERROR);

  glGenFramebuffers(1, &m_nFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

  error = glGetError();
  assert (error == GL_NO_ERROR);

  // attach the texture to FBO color attachment point

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_nAlbedoDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_nNormalDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_nTangentDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_nSpecularDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_nDisplaceDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, m_nPositionDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, m_nNormalMapDrawBuffer, 0);

  /*glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP, m_nAlbedoDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP, m_nNormalDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP, m_nTangentDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_CUBE_MAP, m_nSpecularDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_CUBE_MAP, m_nDisplaceDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_CUBE_MAP, m_nPositionDrawBuffer, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_CUBE_MAP, m_nNormalMapDrawBuffer, 0);*/

  // attach the renderbuffer to depth attachment point
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP, m_nDepthDrawBuffer, 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_nDepthDrawBuffer, 0);

  GLenum tDrawBuffers[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
  glDrawBuffers(7, tDrawBuffers);

  //check fbo status
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if(status != GL_FRAMEBUFFER_COMPLETE)
      Logger::fatal() << "Failed to initialize FBO for RN_Deferred, status=" << TranslateFBOStatus(status) << Logger::endl;


  //release used texture unit
  pTextureManager->ReleaseUnit(nUsedTextureUnit);

  //unbind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  m_bLayeredFBO = true;
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
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
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

GLuint Bamboo::RN_Deferred::ItlCreateLayeredColorTexture()
{
  GLenum error = glGetError();
  assert (error == GL_NO_ERROR);

  GLuint nNewTexture;

  //generate color texture (=create new opengl id)
  glGenTextures(1, &nNewTexture);

  //bind color texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, nNewTexture);

  error = glGetError();
  assert (error == GL_NO_ERROR);

  //set texture format and data
  for (int iFace = 0; iFace < 6; iFace++)
  {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, 0, GL_RGBA32F, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);

      error = glGetError();
      const char * szErrorMessage = TranslateGLerror(error);
      //std::cout << szErrorMessage << std::endl;
      assert (error == GL_NO_ERROR);
  }

  //set texture parameters
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  error = glGetError();
  assert (error == GL_NO_ERROR);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, m_nWidth, m_nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


    //set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return nNewTexture;
}

GLuint Bamboo::RN_Deferred::ItlCreateLayeredDepthTexture()
{
  GLenum error = glGetError();
  assert (error == GL_NO_ERROR);

  GLuint nNewTexture;

  //generate depth texture
  glGenTextures(1, &nNewTexture);

  //bind depth texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, nNewTexture);

  error = glGetError();
  assert (error == GL_NO_ERROR);

  //set texture format and data
  for (int iFace = 0; iFace < 6; iFace++)
  {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, 0, GL_DEPTH_STENCIL, m_nWidth, m_nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    error = glGetError();
    const char * szErrorMessage = TranslateGLerror(error);
   // std::cout << szErrorMessage << std::endl;
    assert (error == GL_NO_ERROR);
  }

  //set texture parameters
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

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
    glDeleteTextures(1, &m_nNormalMapDrawBuffer);
    glDeleteTextures(1, &m_nTangentDrawBuffer);
    glDeleteTextures(1, &m_nSpecularDrawBuffer);
    glDeleteTextures(1, &m_nDisplaceDrawBuffer);
    glDeleteTextures(1, &m_nDepthDrawBuffer);
    glDeleteTextures(1, &m_nPositionDrawBuffer);
}

void Bamboo::RN_Deferred::ItlPreRenderChildren()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    ItlPushFBO(m_nFBO);

    //set viewport size
    glViewport(0,0, m_nWidth, m_nHeight);

    // clear buffers
   // glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
    if (m_bLayeredFBO)
      ItlGetGraphicCore()->GetShaderManager()->ActivateShader("deferred_pass_cm");
    else
      ItlGetGraphicCore()->GetShaderManager()->ActivateShader("deferred_pass");

    GLuint l_nUseParallax = ItlGetGraphicCore()->GetShaderManager()->GetUniform("nUseParallax");

    assert (l_nUseParallax != -1);
    glUniform1i(l_nUseParallax, s_nUseParallax % 2);

    ItlPushViewportInformation(m_nWidth, m_nHeight);

}

void Bamboo::RN_Deferred::ItlPostRenderChildren()
{
    // todo:: fault?
    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();

    //remove the fbo (THIS fbo) from the bound_fbos stack
    ItlPopFBO();

    ItlPopViewportInformation();

    //if there was a SceneObject_FBO bound in the SceneObject_Tree, rebind the previous bound fbo
    if (ItlIsNestedFBO())
    {
        GLuint previous_bound = ItlGetTopFBO();
        glBindFramebuffer(GL_FRAMEBUFFER, previous_bound);
    }
    else
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int iOldWidth, iOldHeight;

    ItlGetTopViewportInformation(iOldWidth, iOldHeight);

    //restore viewport params
    glViewport(0, 0, iOldWidth, iOldHeight);
}

void Bamboo::RN_Deferred::ItlPreRender()
{
}

void Bamboo::RN_Deferred::ItlRender()
{
  if (m_bLayeredFBO == false)
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);
      ItlPushFBO(m_nFBO);
      ItlPushViewportInformation(m_nWidth, m_nHeight);

      glViewport(0,0, m_nWidth, m_nHeight);
      GLenum tDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
      glDrawBuffers(1, tDrawBuffers);
      //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      //glClear(GL_COLOR_BUFFER_BIT);
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 0, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);


      // first, we search for all lights
      for (unsigned int a=0; a < m_vspSpotLights.size(); a++)
      {
          glClear(GL_STENCIL_BUFFER_BIT);

          std::shared_ptr<Bamboo::RN_SpotLight> spLight (m_vspSpotLights[a]);
          spLight->SetTextureLocation("color_texture", m_nAlbedoDrawBuffer);
          spLight->SetTextureLocation("normal_texture", m_nNormalDrawBuffer);
          spLight->SetTextureLocation("tangent_texture", m_nTangentDrawBuffer);
          spLight->SetTextureLocation("normalmap_texture", m_nNormalMapDrawBuffer);
          spLight->SetTextureLocation("depth_texture", m_nDepthDrawBuffer);
          spLight->SetTextureLocation("position_texture", m_nPositionDrawBuffer);
          spLight->SetTextureLocation("specular_texture", m_nSpecularDrawBuffer);
          spLight->SetTextureLocation("displace_texture", m_nDisplaceDrawBuffer);
          //spLight->SetTextureLocation("position_texture", m_nPositionDrawBuffer);

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

      glDisable(GL_STENCIL_TEST);

      GLenum tDrawBuffers2[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 , GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 , GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6};
      glDrawBuffers(7, tDrawBuffers2);


      ItlPostRenderChildren();

      // ---BEGIN ---- this is just for debugging!

      static Bamboo::RN_PostEffect rPostEffectNode("directwrite");

      GLuint nTextureToShow;

     // std::cout << s_DebugDeferredTexture << std::endl;

      switch (s_DebugDeferredTexture % 6)
      {
      case 0:
          nTextureToShow = m_nAlbedoDrawBuffer;
          break;
      case 1:
          nTextureToShow = m_nDisplaceDrawBuffer;
          break;
      case 2:
          nTextureToShow = m_nSpecularDrawBuffer;
          break;
      case 3:
          nTextureToShow = m_nNormalMapDrawBuffer;
          break;
      case 4:
          nTextureToShow = m_nTangentDrawBuffer;
          break;
      case 5:
          nTextureToShow = m_nSpecularDrawBuffer;
          break;
      }

      rPostEffectNode.SetTexture("texture1", nTextureToShow );

      rPostEffectNode.Render();
    }
  else
    {
      /*static bool bAlreadyPrinted = false;
      if (bAlreadyPrinted == false)
        {
          Logger::debug() << "Texture id of cube map: " << m_nAlbedoDrawBuffer << Logger::endl;
          bAlreadyPrinted = true;
        }*/
    }



}

void Bamboo::RN_Deferred::ItlPostRender()
{
}

void Bamboo::RN_Deferred::AddSpotLight(std::shared_ptr<Bamboo::RN_SpotLight> spSpotlight)
{
    m_vspSpotLights.push_back(spSpotlight);
}


