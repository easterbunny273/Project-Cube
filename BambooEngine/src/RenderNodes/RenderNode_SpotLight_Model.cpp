#include <glm/gtc/matrix_transform.hpp>

#include "RenderNodes/RenderNode_SpotLight_Model.h"
#include "PC_Logger.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <iostream>

#define NEARPLANE 0.01f
#define FARPLANE 0.05f
#define SHADOWMAP_RESOLUTION 256.0f

Bamboo::RN_SpotLight_Model::RN_SpotLight_Model(glm::vec3 vPosition,
                                               glm::vec3 vLookDirection,
                                               float fFOV,
                                               glm::vec3 vLightColor)
{
    // create projection matrix
    m_m4ProjectionMatrix    = glm::perspective(fFOV, 1.0f, NEARPLANE, FARPLANE);

    // create view matrix
    m_m4ViewMatrix          = glm::lookAt(vPosition, vPosition + vLookDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    ItlCreateVBO();

    m_vLightColor = vLightColor;

    // prepare vertex array object
    ItlPrepareVAO();

    Logger::debug() << "RN_SpotLight_Model created" << Logger::endl;
}

Bamboo::RN_SpotLight_Model::~RN_SpotLight_Model()
{
  Logger::debug() << "RN_SpotLight_Model destroyed" << Logger::endl;
}

void Bamboo::RN_SpotLight_Model::ItlCreateVBO()
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
}

void Bamboo::RN_SpotLight_Model::ItlPreRender()
{
    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
    ItlGetGraphicCore()->GetShaderManager()->ActivateShader("camera-debug2");
}

void Bamboo::RN_SpotLight_Model::ItlPostRender()
{
    ItlGetGraphicCore()->GetShaderManager()->PopActiveShader();
}

void Bamboo::RN_SpotLight_Model::ItlLoadRessources()
{

}

void Bamboo::RN_SpotLight_Model::ItlRender()
{
    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();

    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);

    const GLint l_cameraInverse_Position = pShaderManager->GetUniform("Camera_InverseMatrix");
    const GLint l_lightcolor_Position = pShaderManager->GetUniform("vLightColor");

    glm::mat4 mViewProjectionMatrix = m_m4ProjectionMatrix * m_m4ViewMatrix;
    glm::mat4 mInverseViewProjectionMatrix = glm::inverse(mViewProjectionMatrix);

    if (l_lightcolor_Position != -1)
        glUniform3f(l_lightcolor_Position, m_vLightColor.r, m_vLightColor.g, m_vLightColor.b);

    if (l_cameraInverse_Position != -1)
        glUniformMatrix4fv(l_cameraInverse_Position, 1, GL_FALSE, &mInverseViewProjectionMatrix[0][0]);

   // glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_TRIANGLES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Bamboo::RN_SpotLight_Model::ItlPrepareVAO()
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

void Bamboo::RN_SpotLight_Model::ItlPreRenderChildren()
{

}

void Bamboo::RN_SpotLight_Model::ItlPostRenderChildren()
{


}
