
#include "PC_Logger.h"
#include "ShaderManager.h"
#include "RenderNodes/RenderNode_RenderPass.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "Camera.h"

extern bool bUseCamera1;

Bamboo::RN_Camera::RN_Camera(Bamboo::ICamera * pCamera, bool bSetMatrices)
    : m_pCamera(pCamera), m_bSetMatrices(bSetMatrices)
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

    /* Vertexbuffer generieren */
    glGenBuffers(1,&m_nVertexBufferObject);
    glGenBuffers(1,&m_nIndexBufferObject);

    /* zum befüllen mal aktivieren ...*/
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    /* befüllen ... */
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vertexArray[0]), vertexArray, GL_STATIC_DRAW);

    /* nun nur noch den Index-Array in den Bufferbereich schreiben */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    m_iIndexArraySize = 24;

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "RN_Camera created" << Logger::endl;
}

Bamboo::RN_Camera::~RN_Camera()
{
  Logger::debug() << "RN_Camera destroyed" << Logger::endl;
}

void Bamboo::RN_Camera::Render(std::shared_ptr<TItlRenderInfo> pCurrentRenderInfo)
{
    //store old matrices
    if (m_bSetMatrices)// || !bUseCamera1)
    {
	glm::mat4 SavedProjectionMatrix = pCurrentRenderInfo->ProjectionMatrix;
	glm::mat4 SavedViewMatrix = pCurrentRenderInfo->ViewMatrix;
	glm::mat4 SavedTranslationMatrix = pCurrentRenderInfo->TranslationMatrix;
	glm::mat4 SavedModelViewProjectionMatrix = pCurrentRenderInfo->ModelViewProjectionMatrix;


	pCurrentRenderInfo->ProjectionMatrix = m_pCamera->GetProjectionMatrix();
	pCurrentRenderInfo->ViewMatrix = m_pCamera->GetViewMatrix();
	pCurrentRenderInfo->TranslationMatrix = m_pCamera->GetTranslationMatrix();
	pCurrentRenderInfo->ModelViewProjectionMatrix = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix();
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix;

        Bamboo::IRenderNode::Render(pCurrentRenderInfo);

	pCurrentRenderInfo->ProjectionMatrix = SavedProjectionMatrix;
	pCurrentRenderInfo->ViewMatrix = SavedViewMatrix;
	pCurrentRenderInfo->TranslationMatrix = SavedTranslationMatrix;
	pCurrentRenderInfo->ModelViewProjectionMatrix = SavedModelViewProjectionMatrix;
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix;
    }
    else
    {
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix();
        Bamboo::IRenderNode::Render(pCurrentRenderInfo);
	pCurrentRenderInfo->ModelViewProjectionMatrix_ForFrustumCulling = pCurrentRenderInfo->ModelViewProjectionMatrix;
    }
}

void Bamboo::RN_Camera::ItlRender()
{
   /* const GLint l_in_Position(ShaderManager::instance()->GetAttribute("in_Position"));
    const GLint l_cameraInverse_Position = ShaderManager::instance()->GetUniform("Camera_InverseMatrix");

    glm::mat4 mInverseViewProjectionMatrix = glm::inverse(m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix());

    if (l_cameraInverse_Position != -1)
	glUniformMatrix4fv(l_cameraInverse_Position, 1, GL_FALSE, &mInverseViewProjectionMatrix[0][0]);

    if (l_in_Position != -1)
    {
	glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
	glEnableVertexAttribArray(l_in_Position);
    }

    //float fPreviousLineWidth;
    //int iPreviousPolygonMode;

    //glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);
    //glGetIntegerv(GL_POLYGON_MODE, &iPreviousPolygonMode);

    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_LINES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

   // glLineWidth(fPreviousLineWidth);
    //glPolygonMode(GL_FRONT_AND_BACK, iPreviousPolygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
}

void Bamboo::RN_Camera::ItlPreRender()
{
   /* glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->PushActiveShader();
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->ActivateShader("camera-debug");
    else
        ShaderManager::instance()->ActivateShader("camera-debug");*/
}

void Bamboo::RN_Camera::ItlPostRender()
{
   // ShaderManager::instance()->PopActiveShader();
}

