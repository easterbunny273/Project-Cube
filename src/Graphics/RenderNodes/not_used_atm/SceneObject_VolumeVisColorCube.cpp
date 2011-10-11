/*
 * source file for SceneObject_VolumeVisColorCube class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 05/2011
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
#include "Graphics/SceneObjects/SceneObject_VolumeVisColorCube.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"

using namespace std;

const double PI = 3.141516;

SceneObject_VolumeVisColorCube::SceneObject_VolumeVisColorCube(GLint iCullMode)
    : m_iCullMode(iCullMode), SceneObject()
{
    assert (iCullMode == GL_FRONT || iCullMode == GL_BACK);

    GLdouble *vertexArray;
    GLuint *indexArray;

    vertexArray = new GLdouble[8 * 3];

    const float fMinX = -1.0f;
    const float fMinY = -1.0f;
    const float fMinZ = -1.0f;
    const float fMaxX = 1.0f;
    const float fMaxY = 1.0f;
    const float fMaxZ = 1.0f;

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


    indexArray = new GLuint[36];

    //2 triangles vorne
    indexArray[0] = 0;
    indexArray[1] = 1;
    indexArray[2] = 2;

    indexArray[3] = 2;
    indexArray[4] = 3;
    indexArray[5] = 0;

    //2 triangles hinten
    indexArray[6] = 7;
    indexArray[7] = 6;
    indexArray[8] = 5;

    indexArray[9] = 5;
    indexArray[10] = 4;
    indexArray[11] = 7;

    //2 triangles links
    indexArray[12] = 4;
    indexArray[13] = 5;
    indexArray[14] = 1;

    indexArray[15] = 1;
    indexArray[16] = 0;
    indexArray[17] = 4;

    //2 triangles rechts
    indexArray[18] = 3;
    indexArray[19] = 2;
    indexArray[20] = 6;

    indexArray[21] = 6;
    indexArray[22] = 7;
    indexArray[23] = 3;

    //2 triangles oben
    indexArray[24] = 1;
    indexArray[25] = 5;
    indexArray[26] = 6;

    indexArray[27] = 6;
    indexArray[28] = 2;
    indexArray[29] = 1;

    //2 triangles unten
    indexArray[30] = 0;
    indexArray[31] = 3;
    indexArray[32] = 7;

    indexArray[33] = 7;
    indexArray[34] = 4;
    indexArray[35] = 0;


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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    m_iIndexArraySize = 36;

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "SceneObject_VolumeVisColorCube created" << Logger::endl;
}

SceneObject_VolumeVisColorCube::~SceneObject_VolumeVisColorCube()
{

}

void SceneObject_VolumeVisColorCube::ItlPreRender()
{
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->pushActiveShader();
    ShaderManager::instance()->activateShader("colorcube");
}

void SceneObject_VolumeVisColorCube::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();
}

void SceneObject_VolumeVisColorCube::ItlRender()
{
    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));

    if (l_in_Position != -1)
    {
	glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), NULL);
	glEnableVertexAttribArray(l_in_Position);
    }

    float fPreviousLineWidth;
    int iPreviousCullMode;
    int iPreviousCullFace;

 //   glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);
   // glGetIntegerv(GL_CULL_FACE_MODE, &iPreviousCullMode);
    //glGetIntegerv(GL_CULL_FACE, &iPreviousCullFace);

    glEnable(GL_CULL_FACE);
    glCullFace(m_iCullMode);

    glLineWidth(1.0f);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_TRIANGLES, m_iIndexArraySize, GL_UNSIGNED_INT, NULL);

    glLineWidth(fPreviousLineWidth);

    glCullFace(GL_BACK);
    //glCullFace(iPreviousCullFace);

}

