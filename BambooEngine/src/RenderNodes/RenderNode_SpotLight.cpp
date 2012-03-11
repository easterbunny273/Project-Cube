#include <glm/gtc/matrix_transform.hpp>

#include "RenderNodes/RenderNode_SpotLight.h"
#include "PC_Logger.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <iostream>

#define NEARPLANE 0.1f
#define FARPLANE 50.0f
#define SHADOWMAP_RESOLUTION 512.0f

Bamboo::RN_SpotLight::RN_SpotLight(glm::vec3 vPosition,
                                   glm::vec3 vLookDirection,
                                   float fFOV,
                                   glm::vec3 vLightColor)
{
    // create projection matrix
    m_m4ProjectionMatrix    = glm::perspective(fFOV, 1.0f, NEARPLANE, FARPLANE);

    // create view matrix
    m_m4ViewMatrix          = glm::lookAt(vPosition, vPosition + vLookDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    // create fbo
    ItlCreateFBO();

    ItlCreateVBO();

    m_vLightColor = vLightColor;
    m_vLightPosition = vPosition;


    Logger::debug() << "RN_SpotLight created" << Logger::endl;
}

Bamboo::RN_SpotLight::~RN_SpotLight()
{
    glDeleteFramebuffers(1, &m_nFBO);
    glDeleteTextures(1, &m_nDepthTexture);

    Logger::debug() << "RN_SpotLight destroyed" << Logger::endl;
}

void Bamboo::RN_SpotLight::ItlCreateVBO()
{
    GLdouble *vertexArray;
    GLuint *indexArray;

    vertexArray = new GLdouble[8 * 3];

    //0 links unten vorne
    vertexArray[0] = -1.0f; vertexArray[1] = -1.0f; vertexArray[2] = -1.0f;

    //1 links oben vorne
    vertexArray[3] = -1.0f; vertexArray[4] = 1.0f; vertexArray[5] = -1.0f;

    //2 rechts oben vorne
    vertexArray[6] = 1.0f; vertexArray[7] = 1.0f; vertexArray[8] = -1.0f;

    //3 rechts unten vorne
    vertexArray[9] = 1.0f; vertexArray[10] = -1.0f; vertexArray[11] = -1.0f;

    //4 links unten hinten
    vertexArray[12] = -1.0f; vertexArray[13] = -1.0f; vertexArray[14] = 1.0f;

    //5 links oben hinten
    vertexArray[15] = -1.0f; vertexArray[16] = 1.0f; vertexArray[17] = 1.0f;

    //6 rechts oben hinten
    vertexArray[18] = 1.0f; vertexArray[19] = 1.0f; vertexArray[20] = 1.0f;

    //7 rechts unten hinten
    vertexArray[21] = 1.0f; vertexArray[22] = -1.0f; vertexArray[23] = 1.0f;


    indexArray = new GLuint[36];

    // links
    indexArray[0] = 5;
    indexArray[1] = 1;
    indexArray[2] = 4;

    indexArray[3] = 4;
    indexArray[4] = 1;
    indexArray[5] = 0;

    //hingen
    indexArray[6] = 5;
    indexArray[7] = 4;
    indexArray[8] = 6;

    indexArray[9] = 7;
    indexArray[10] = 6;
    indexArray[11] = 4;

    //rechts
    indexArray[12] = 6;
    indexArray[13] = 7;
    indexArray[14] = 2;

    indexArray[15] = 3;
    indexArray[16] = 2;
    indexArray[17] = 7;

    // vorne
    indexArray[18] = 1;
    indexArray[19] = 0;
    indexArray[20] = 2;

    indexArray[21] = 2;
    indexArray[22] = 0;
    indexArray[23] = 3;

    //oben
    indexArray[24] = 2;
    indexArray[25] = 1;
    indexArray[26] = 6;

    indexArray[27] = 5;
    indexArray[28] = 6;
    indexArray[29] = 1;

    //unten
    indexArray[30] = 0;
    indexArray[31] = 3;
    indexArray[32] = 4;

    indexArray[33] = 7;
    indexArray[34] = 4;
    indexArray[35] = 3;




    glGenVertexArrays(1, &m_nVertexArrayObject);
    glBindVertexArray(m_nVertexArrayObject);

    // Vertexbuffer generieren
    glGenBuffers(1,&m_nVertexBufferObject);
    glGenBuffers(1,&m_nIndexBufferObject);

    // zum befüllen mal aktivieren ...
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    // befüllen ...
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vertexArray[0]), vertexArray, GL_STATIC_DRAW);

    // nun nur noch den Index-Array in den Bufferbereich schreiben
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    m_iIndexArraySize = 36;

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
      Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    // prepare the vertex array object
    ItlPrepareVAO();
}

void Bamboo::RN_SpotLight::ItlPrepareVAO()
{
  // get shaer manager
  ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();

  // set "our" shader
  pShaderManager->PushActiveShader();
  pShaderManager->ActivateShader("light-pass");

  // get memory position of attribute
  const GLint l_in_Position(pShaderManager->GetAttribute("in_Position"));
  assert (l_in_Position != -1);

  // bind vertex array object
  glBindVertexArray(m_nVertexArrayObject);

  // bind vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

  // bind data in vbo to "in_Position" and enable attribute
  if (l_in_Position != -1)
  {
      glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
      glEnableVertexAttribArray(l_in_Position);
  }

  // reset active shader
  pShaderManager->PopActiveShader();
}

GLuint Bamboo::RN_SpotLight::ItlCreateColorTexture()
{
    GLuint nNewTexture;

    //generate color texture (=create new opengl id)
    glGenTextures(1, &nNewTexture);

    //bind color texture
    glBindTexture(GL_TEXTURE_2D, nNewTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION, 0, GL_RGBA, GL_FLOAT, 0);
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

GLuint Bamboo::RN_SpotLight::ItlCreateDepthTexture()
{
    GLuint nNewTexture;

    //generate depth texture
    glGenTextures(1, &nNewTexture);

    //bind depth texture
    glBindTexture(GL_TEXTURE_2D, nNewTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

void Bamboo::RN_SpotLight::ItlCreateFBO()
{
    TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();

    m_nColorTexture = ItlCreateColorTexture();
    m_nDepthTexture = ItlCreateDepthTexture();

    //get the id of a free texture unit from the texture manager
    GLuint nUsedTextureUnit = pTextureManager->RequestFreeUnit(); //ask for a free texture unit

    //activate unit
    glActiveTexture(GL_TEXTURE0 + nUsedTextureUnit);

    glGenFramebuffers(1, &m_nFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nColorTexture, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthTexture, 0);

    //check fbo status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
        Logger::fatal() << "Failed to initialize FBO for RN_Spotlight" << Logger::endl;
    else
        Logger::debug() << "Created FBO for SpotLight" << Logger::endl;

    //unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    pTextureManager->ReleaseUnit(nUsedTextureUnit);
}

void Bamboo::RN_SpotLight::ItlPreRender()
{
    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
    ItlGetGraphicCore()->GetShaderManager()->ActivateShader("light-pass");
}

void Bamboo::RN_SpotLight::ItlPostRender()
{
    ItlGetGraphicCore()->GetShaderManager()->PopActiveShader();


}

void Bamboo::RN_SpotLight::ItlLoadRessources()
{

}

void Bamboo::RN_SpotLight::ItlRender()
{


    glm::mat4 mViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;
    glm::mat4 mInverseViewProjectionMatrix = glm::inverse(mViewProjectionMatrix);

    //if (mViewProjectionMatrix != m_m4LastUsedViewProjectionMatrix || m_vLightPosition != m_v3LastUsedLightPosition)
      //m_bHasChildren = (!m_vChildren.empty());



      glDisable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE);

      glBindVertexArray(m_nVertexArrayObject);
      glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

      ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
      TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();

      const GLint l_cameraInverse_Position = pShaderManager->GetUniform("Camera_InverseMatrix");
      const GLint l_lightmatrix_Position = pShaderManager->GetUniform("Light_ViewProjectionMatrix");
      const GLint l_shadowmap_Position = pShaderManager->GetUniform("shadowmap");
      const GLint l_spotmask_Position = pShaderManager->GetUniform("spotmask");
      const GLint l_lightcolor_Position = pShaderManager->GetUniform("vLightColor");
      const GLint l_lightposition_Position = pShaderManager->GetUniform("LightPosition");




      if (l_lightposition_Position != -1)
          glUniform3f(l_lightposition_Position, m_vLightPosition.x, m_vLightPosition.y, m_vLightPosition.z);

      if (l_lightcolor_Position != -1)
          glUniform3f(l_lightcolor_Position, m_vLightColor.r, m_vLightColor.g, m_vLightColor.b);

      if (l_cameraInverse_Position != -1)
          glUniformMatrix4fv(l_cameraInverse_Position, 1, GL_FALSE, &mInverseViewProjectionMatrix[0][0]);

      if (l_lightmatrix_Position != -1)
          glUniformMatrix4fv(l_lightmatrix_Position, 1, GL_FALSE, &mViewProjectionMatrix[0][0]);

      GLuint iTextureUnitShadowMap = pTextureManager->UseTexture(m_nColorTexture);
      GLuint iTextureUnitSpotMask = pTextureManager->UseTexture("spotlight");

      if (l_spotmask_Position != -1)
          glUniform1i(l_spotmask_Position, iTextureUnitSpotMask);

      if (l_shadowmap_Position != -1)
          glUniform1i(l_shadowmap_Position, iTextureUnitShadowMap);

      for (auto iter=m_mTextureLocations.begin(); iter!= m_mTextureLocations.end(); iter++)
      {
          const std::string *pName = &(iter->first);
          GLint iTextureID = iter->second;

          GLuint iTextureUnit = pTextureManager->UseTexture(iTextureID);

          GLint iUniformLocation = pShaderManager->GetUniform(*pName);
          if (iUniformLocation != -1)
              glUniform1i(iUniformLocation, iTextureUnit);
      }

     // glLineWidth(1.0f);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      //glDisable(GL_CULL_FACE);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
      glDrawElements(GL_TRIANGLES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

      //glEnable(GL_CULL_FACE);
      //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glBindVertexArray(0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      pTextureManager->UnuseTexture(m_nColorTexture);
      pTextureManager->UnuseTexture("spotlight");

      for (auto iter=m_mTextureLocations.begin(); iter!= m_mTextureLocations.end(); iter++)
      {
          GLint iTextureID = iter->second;
          pTextureManager->UnuseTexture(iTextureID);
      }

      glEnable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);

}

void Bamboo::RN_SpotLight::ItlPreRenderChildren()
{
    glm::mat4 mViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;

   // m_m4LastUsedViewProjectionMatrix = glm::mat4();

    //if (mViewProjectionMatrix != m_m4LastUsedViewProjectionMatrix || m_vLightPosition != m_v3LastUsedLightPosition)
    if (true)
    {
        //bind fbo
        glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

        //set viewport size
        glViewport(0,0, SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

        // clear buffers
       // glClearColor(0.0, 0.0, 0.0, 0.0);
        //glClearDepth(1.0);
        glClear(GL_DEPTH_BUFFER_BIT);

        ///store our fbo-id on the stack, to allow "deeper" SceneObject_FBOs to rebind our fbo after finish drawing
        ItlPushFBO(m_nFBO);

        // change matrices, so that the scene is rendered from the view of the light
        ItlChangeMatrices();

        ItlPushViewportInformation(SHADOWMAP_RESOLUTION, SHADOWMAP_RESOLUTION);

        m_bHasChildren = (!m_vChildren.empty());
    }
    else
    {
        m_bHasChildren = false;
    }
}

void Bamboo::RN_SpotLight::ItlPostRenderChildren()
{
    glm::mat4 mViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;

    //if (mViewProjectionMatrix != m_m4LastUsedViewProjectionMatrix || m_vLightPosition != m_v3LastUsedLightPosition)
    if (true)
    {
      // restore matrices
      ItlRestoreMatrices();

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

      m_m4LastUsedViewProjectionMatrix = mViewProjectionMatrix;
      m_v3LastUsedLightPosition = m_vLightPosition;
    }
}

void Bamboo::RN_SpotLight::ItlChangeMatrices()
{
    m_m4SavedProjectionMatrix = m_pCurrentRenderInfo->ProjectionMatrix;
    m_m4SavedViewMatrix = m_pCurrentRenderInfo->ViewMatrix;
    m_m4SavedViewProjectionMatrix = m_pCurrentRenderInfo->ModelViewProjectionMatrix;

    m_pCurrentRenderInfo->ProjectionMatrix = m_m4ProjectionMatrix;
    m_pCurrentRenderInfo->ViewMatrix = m_m4ViewMatrix;
    m_pCurrentRenderInfo->ModelViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;
    m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = m_pCurrentRenderInfo->ModelViewProjectionMatrix;
}

void Bamboo::RN_SpotLight::ItlRestoreMatrices()
{
    m_pCurrentRenderInfo->ProjectionMatrix = m_m4SavedProjectionMatrix;
    m_pCurrentRenderInfo->ViewMatrix = m_m4SavedViewMatrix;
    m_pCurrentRenderInfo->ModelViewProjectionMatrix = m_m4SavedViewProjectionMatrix;
    m_pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = m_pCurrentRenderInfo->ModelViewProjectionMatrix;


}

void Bamboo::RN_SpotLight::SetTextureLocation(std::string sTexture, GLint iLocation)
{
    m_mTextureLocations[sTexture] = iLocation;
}

/*
void Bamboo::RN_SpotLight::Render(std::shared_ptr<TItlRenderInfo> pCurrentRenderInfo)
{
    //store old matrices
    glm::mat4 SavedProjectionMatrix = pCurrentRenderInfo->ProjectionMatrix;
    glm::mat4 SavedViewMatrix = pCurrentRenderInfo->ViewMatrix;
    glm::mat4 SavedModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix;

    pCurrentRenderInfo->ProjectionMatrix = m_m4ProjectionMatrix;
    pCurrentRenderInfo->ViewMatrix = m_m4ViewMatrix;
    pCurrentRenderInfo->ModelViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;
    pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix;

    Bamboo::IRenderNode::Render(pCurrentRenderInfo);

    pCurrentRenderInfo->ProjectionMatrix = SavedProjectionMatrix;
    pCurrentRenderInfo->ViewMatrix = SavedViewMatrix;
    pCurrentRenderInfo->ModelViewProjectionMatrix = SavedModelViewProjectionMatrix;
    pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix;
}*/
