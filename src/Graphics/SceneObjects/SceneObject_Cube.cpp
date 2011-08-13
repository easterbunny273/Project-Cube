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
#include "Graphics/SceneObjects/SceneObject_Cube.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"

using namespace std;

const double PI = 3.141516;

SceneObject_Cube::SceneObject_Cube()
    : SceneObject()
{
    //GLdouble *vertexArray;
    //GLuint *indexArray;

    const GLdouble pdVerticeData[] = {
	-1.0, -1.0, -1.0,
	0.0, 0.0, 0.0,	    //texcoord
	-1.0, 1.0, -1.0,
	0.0, 1.0, 0.0,	    //texcoord
	-1.0, -1.0, 1.0,
	1.0, 0.0, 0.0,	    //texcoord

	-1.0, 1.0, -1.0,
	0.0, 1.0, 0.0,	    //texcoord
	-1.0, 1.0, 1.0,
	1.0, 1.0, 0.0,	    //texcoord
	-1.0, -1.0, 1.0,
	1.0, 0.0, 0.0,	    //texcoord

	// #############################
	-1.0, -1.0, 1.0,
	0.0, 0.0, 0.0,	    //texcoord
	-1.0, 1.0, 1.0,
	0.0, 1.0, 0.0,	    //texcoord
	1.0, -1.0, 1.0,
	1.0, 0.0, 0.0,	    //texcoord

	-1.0, 1.0, 1.0,
	0.0, 1.0, 0.0,	//texcoord
	1.0, 1.0, 1.0,
	1.0, 1.0, 0.0,	    //texcoord
	1.0, -1.0, 1.0,
	1.0, 0.0, 0.0,	    //texcoord

	// #############################
	1.0, -1.0, 1.0,
	0.0, 0.0, 0.0,	    //texcoord
	1.0, 1.0, 1.0,
	0.0, 1.0, 0.0,	    //texcoord
	1.0, -1.0, -1.0,
	1.0, 0.0, 0.0,	    //texcoord

	1.0, 1.0, 1.0,
	0.0, 1.0, 0.0,	//texcoord
	1.0, 1.0, -1.0,
	1.0, 1.0, 0.0,	    //texcoord
	1.0, -1.0, -1.0,
	1.0, 0.0, 0.0,	    //texcoord

	// #############################
	1.0, -1.0, -1.0,
	0.0, 0.0, 0.0,	    //texcoord
	1.0, 1.0, -1.0,
	0.0, 1.0, 0.0,	    //texcoord
	-1.0, -1.0, -1.0,
	1.0, 0.0, 0.0,	    //texcoord

	1.0, 1.0, -1.0,
	0.0, 1.0, 0.0,	//texcoord
	-1.0, 1.0, -1.0,
	1.0, 1.0, 0.0,	    //texcoord
	-1.0, -1.0, -1.0,
	1.0, 0.0, 0.0	    //texcoord

    };


    glGenVertexArrays(1, &m_nVertexArrayObject);
    glBindVertexArray(m_nVertexArrayObject);

    /* Vertexbuffer generieren */
    glGenBuffers(1,&m_nVertexBufferObject);
    //glGenBuffers(1,&m_nIndexBufferObject);

    /* zum befüllen mal aktivieren ...*/
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    /* befüllen ... */
    glBufferData(GL_ARRAY_BUFFER, 144 * sizeof(pdVerticeData[0]), pdVerticeData, GL_STATIC_DRAW);

    /* nun nur noch den Index-Array in den Bufferbereich schreiben */
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

   // m_iIndexArraySize = 24;

    //delete[] vertexArray;
    //delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
	Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    // load ressources
    ItlLoadRessources();

    Logger::debug() << "SceneObject_Cube created" << Logger::endl;
}

SceneObject_Cube::~SceneObject_Cube()
{

}

void SceneObject_Cube::ItlPreRender()
{
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->PushActiveShader();
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->ActivateShader("bounding_shader");
    else
	ShaderManager::instance()->ActivateShader("basic_shading");
}

void SceneObject_Cube::ItlPostRender()
{
    ShaderManager::instance()->PopActiveShader();
}

void SceneObject_Cube::ItlRender()
{
    GLenum eError = glGetError();

    if (eError != GL_NO_ERROR)
	Logger::error() << TranslateGLerror(eError) << Logger::endl;

    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    const GLint l_in_Position(ShaderManager::instance()->GetAttribute("in_Position"));

    if (l_in_Position != -1)
    {
	glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
	glEnableVertexAttribArray(l_in_Position);
    }

    const GLint l_in_Normal(ShaderManager::instance()->GetAttribute("in_Normal"));

    if (l_in_Normal != -1)
    {
	//glVertexAttribPointer(l_in_Normal, 3, GL_DOUBLE, GL_FALSE, 9 * sizeof(GLdouble),  (const GLvoid *)(6 * sizeof(GLdouble)));
	//glEnableVertexAttribArray(l_in_Normal);
    }

    const GLint l_in_Texcoord(ShaderManager::instance()->GetAttribute("in_Texcoord"));

    if (l_in_Texcoord != -1)
    {
	glVertexAttribPointer(l_in_Texcoord, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble),  (const GLvoid *)(3 * sizeof(GLdouble)));
	glEnableVertexAttribArray(l_in_Texcoord);
    }

    eError = glGetError();

    if (eError != GL_NO_ERROR)
	Logger::error() << TranslateGLerror(eError) << Logger::endl;

    // load texture in texture unit
    GLuint nUsedUnit = TextureManager::instance()->UseTexture("cube_face_back");

    const GLint l_texture1(ShaderManager::instance()->GetUniform("texture1"));

    assert (l_texture1 != -1);

    if (l_texture1 != -1)
	glUniform1i(l_texture1, nUsedUnit);

    glDrawArrays(GL_TRIANGLES, 0, 144);

    // release used texture unit
    TextureManager::instance()->UnuseTexture("cube_face_back");
}

void SceneObject_Cube::ItlLoadRessources()
{
    TextureManager::instance()->LoadTexture("cube_face_left", "textures/cube_left.jpg", false);
    TextureManager::instance()->LoadTexture("cube_face_right", "textures/cube_right.jpg", false);
    TextureManager::instance()->LoadTexture("cube_face_front", "textures/cube_front.jpg", false);
    TextureManager::instance()->LoadTexture("cube_face_back", "textures/cube_back.jpg", false);
}
