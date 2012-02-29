#include "Graphic.h"
#include "RenderNodes/RenderNode_CubeMap.h"
#include "TextureManager.h"
#include "PC_Logger.h"

Bamboo::RN_CubeMap::RN_CubeMap(glm::vec3 vPosition, GLuint nResolution /* = 512*/)
{
  m_v3Position = vPosition;
  m_nResolution = nResolution;

  ItlCreateFBO();
}

Bamboo::RN_CubeMap::~RN_CubeMap()
{

}

void Bamboo::RN_CubeMap::ItlCreateFBO()
{
  TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();

  //get the id of a free texture unit from the texture manager
  GLuint textureUnit = pTextureManager->RequestFreeUnit();

  //activate this unit
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glEnable(GL_TEXTURE_CUBE_MAP);


  // ** CREATE COLOR CUBE TEXTURE **
  // *******************************
  glGenTextures(1, &m_nCubeColorTexture);

  //bind cube texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_nCubeColorTexture);

  // create faces
  for (int iFace = 0; iFace < 6; iFace++)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, 0, GL_RGBA, m_nResolution, m_nResolution, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  //set texture parameters
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  //unbind texture
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


  // ** CREATE DEPTH CUBE TEXTURE **
  // *******************************

  glGenTextures(1, &m_nCubeDepthTexture);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_nCubeDepthTexture);

  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (int iFace = 0; iFace < 6; iFace++)
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, 0, GL_DEPTH_COMPONENT, m_nResolution, m_nResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  //release unit
  pTextureManager->ReleaseUnit(textureUnit);

  // ** CREATE FBO **
  // *******************************

  /*//generate renderbuffer
  glGenRenderbuffers(1, &m_nDepthBuffer);

  //bind renderbuffer
  glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBuffer);

  //set renderbuffer format and data
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_nResolution, m_nResolution);

  //unbind renderbuffer
  glBindRenderbuffer(GL_RENDERBUFFER, 0);*/

  //generate frame buffer
  glGenFramebuffers(1, &m_nFBO);

  //bind framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

  //attach color texture to framebuffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_nCubeColorTexture, 0);
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP, m_nCubeColorTexture, 0);

  //attach depth renderbuffer to framebuffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_nCubeDepthTexture, 0);
  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP, m_nCubeDepthTexture, 0);
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBuffer);

  //check framebuffer status
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  //check if its complete
  if(status != GL_FRAMEBUFFER_COMPLETE)
      Logger::fatal() << "Failed to initialize CubeMap FBO, status=" << status << Logger::endl;
  else
      Logger::debug() << "Initialized CubeMap FBO" << Logger::endl;

  //unbind framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bamboo::RN_CubeMap::ItlPreRenderChildren()
{
  //bind fbo
  glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

  //set viewport size
  glViewport(0,0, m_nResolution, m_nResolution);

  // clear buffers
 // glClearColor(0.0, 0.0, 0.0, 0.0);
  //glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ///store our fbo-id on the stack, to allow "deeper" SceneObject_FBOs to rebind our fbo after finish drawing
  ItlPushFBO(m_nFBO);

  ItlPushViewportInformation(m_nResolution, m_nResolution);
}

void Bamboo::RN_CubeMap::ItlPostRenderChildren()
{
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

  int iOldHeight, iOldWidth;

  ItlGetTopViewportInformation(iOldWidth, iOldHeight);

  //restore viewport params
  glViewport(0, 0, iOldWidth, iOldHeight);
}

void Bamboo::RN_CubeMap::ItlLoadRessources()
{

}
