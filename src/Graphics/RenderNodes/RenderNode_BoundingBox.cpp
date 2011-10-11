/*
 * source file for SceneObject_Sphere class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010
 */
#include <string>
#include <vector>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "Graphics/TextureManager.h"
#include "Logger.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/RenderNodes/RenderNode_BoundingBox.h"
#include "Graphics/RenderNodes/RenderNode_RenderPass.h"

using namespace std;

const double PI = 3.141516;

Graphic::RN_BoundingBox::RN_BoundingBox(float fMinX, float fMaxX, float fMinY, float fMaxY, float fMinZ, float fMaxZ)
    : IRenderNode()
{
    GLdouble *vertexArray;
    GLuint *indexArray;

    vertexArray = new GLdouble[8 * 3];

    //0 links unten vorne
    vertexArray[0] = fMinX; vertexArray[1] = fMinY; vertexArray[2] = fMinZ;

    //1 links oben vorne
    vertexArray[3] = fMinX; vertexArray[4] = fMaxY; vertexArray[5] = fMinZ;

    //2 rechts oben vorne
    vertexArray[6] = fMaxX; vertexArray[7] = fMaxY; vertexArray[8] = fMinZ;

    //3 rechts unten vorne
    vertexArray[9] = fMaxX; vertexArray[10] = fMinY; vertexArray[11] = fMinZ;

    //4 links unten hinten
    vertexArray[12] = fMinX; vertexArray[13] = fMinY; vertexArray[14] = fMaxZ;

    //5 links oben hinten
    vertexArray[15] = fMinX; vertexArray[16] = fMaxY; vertexArray[17] = fMaxZ;

    //6 rechts oben hinten
    vertexArray[18] = fMaxX; vertexArray[19] = fMaxY; vertexArray[20] = fMaxZ;

    //7 rechts unten hinten
    vertexArray[21] = fMaxX; vertexArray[22] = fMinY; vertexArray[23] = fMaxZ;


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

    Logger::debug() << "SceneObject_BoundingBox created" << Logger::endl;
}

Graphic::RN_BoundingBox::~RN_BoundingBox()
{

}

void Graphic::RN_BoundingBox::ItlPreRender()
{
  /*  glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->PushActiveShader();
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->ActivateShader("bounding_shader");
    else
        ShaderManager::instance()->ActivateShader("bounding_shader");*/
}

void Graphic::RN_BoundingBox::ItlPostRender()
{
  //  ShaderManager::instance()->PopActiveShader();
}

void Graphic::RN_BoundingBox::ItlRender()
{
  /*  const GLint l_in_Position(ShaderManager::instance()->GetAttribute("in_Position"));

    if (l_in_Position != -1)
    {
	glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
	glEnableVertexAttribArray(l_in_Position);
    }

    //float fPreviousLineWidth;

    //glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);

    //glLineWidth(1.0f);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
  //  glDrawElements(GL_LINES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

    //glLineWidth(fPreviousLineWidth);*/
}
