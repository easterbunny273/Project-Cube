#include <glm/gtc/matrix_transform.hpp>

#include "RenderNodes/RenderNode_SpotLight.h"
#include "Logger.h"
#include "ShaderManager.h"
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

    Logger::debug() << "RN_SpotLight created" << Logger::endl;
}

Bamboo::RN_SpotLight::~RN_SpotLight()
{
    glDeleteFramebuffers(1, &m_nFBO);
    glDeleteTextures(1, &m_nDepthTexture);
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


    indexArray = new GLuint[24];

    //alle linien vorne
    indexArray[0] = 0;
    indexArray[1] = 1;

    indexArray[2] = 1;
    indexArray[3] = 2;

    indexArray[4] = 2;
    indexArray[5] = 3;

    indexArray[6] = 3;
    indexArray[7] = 0;

    //alle linien hinten
    indexArray[8] = 4;
    indexArray[9] = 5;

    indexArray[10] = 5;
    indexArray[11] = 6;

    indexArray[12] = 6;
    indexArray[13] = 7;

    indexArray[14] = 7;
    indexArray[15] = 4;

    //alle linien links
    indexArray[16] = 0;
    indexArray[17] = 4;

    indexArray[18] = 1;
    indexArray[19] = 5;

    //alle linien rechts
    indexArray[20] = 2;
    indexArray[21] = 6;

    indexArray[22] = 3;
    indexArray[23] = 7;


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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    m_iIndexArraySize = 24;

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;
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
    m_nDepthTexture = ItlCreateDepthTexture();

    glGenFramebuffers(1, &m_nFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFBO);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthTexture, 0);

    //check fbo status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
        Logger::fatal() << "Failed to initialize FBO for RN_Deferred" << Logger::endl;
    else
        Logger::debug() << "Created FBO for SpotLight" << Logger::endl;

    //unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bamboo::RN_SpotLight::ItlPreRender()
{
    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
    ItlGetGraphicCore()->GetShaderManager()->ActivateShader("camera-debug");
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
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();

    const GLint l_in_Position(pShaderManager->GetAttribute("in_Position"));
    const GLint l_cameraInverse_Position = pShaderManager->GetUniform("Camera_InverseMatrix");

    glm::mat4 mInverseViewProjectionMatrix = glm::inverse(m_m4ProjectionMatrix * m_m4ViewMatrix);

    if (l_cameraInverse_Position != -1)
        glUniformMatrix4fv(l_cameraInverse_Position, 1, GL_FALSE, &mInverseViewProjectionMatrix[0][0]);

    if (l_in_Position != -1)
    {
        glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
        glEnableVertexAttribArray(l_in_Position);
    }

   // glLineWidth(1.0f);
   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_LINES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Bamboo::RN_SpotLight::ItlPreRenderChildren()
{
    //save current viewport params
    glGetIntegerv(GL_VIEWPORT, m_iGeneralViewportParams);

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
}

void Bamboo::RN_SpotLight::ItlPostRenderChildren()
{
    // restore matrices
    ItlRestoreMatrices();

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

    glPassThrough(0.3f);
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
