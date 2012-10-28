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
#include "Settings.h"
#include "Graphics/SceneObjects/SceneObject_Sphere.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"

using namespace std;

const double PI = 3.141516;

SceneObject_Sphere::SceneObject_Sphere(float fRadius, const char *szTextureName)
    : SceneObject_BoundingBoxed(), m_szTextureName(szTextureName), m_pVertices(NULL), m_pIndices(NULL)
{
    int i, j;
    int t=0;	//for index array
    int a, b;
    const int lats=24;
    const int longs=24;
    vector<GLfloat> puffer;
    vector<GLuint> pufferInd;

    for(i = 1; i <= lats; i++)
    {
	    double lat_n1 = PI * (-0.5 + (double) (i - 2) / lats);
	    double z_n1  = sin(lat_n1);
	    double zr_n1 =  cos(lat_n1);

	    double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
	    double z0  = sin(lat0);
	    double zr0 =  cos(lat0);

	    double lat1 = PI * (-0.5 + (double) i / lats);
	    double z1 = sin(lat1);
	    double zr1 = cos(lat1);

	    double lat2 = PI * (-0.5 + (double) (i+1) / lats);
	    double z2 = sin(lat2);
	    double zr2 = cos(lat2);

	    for(j = 0; j <= longs; j++)
	    {
		    double lng = 2 * PI * (double) (j - 1) / longs;
		    double x = cos(lng);
		    double y = sin(lng);

		    puffer.push_back(x * zr0 * fRadius);	    //x
		    puffer.push_back(y * zr0 * fRadius);	    //y
		    puffer.push_back(-z0 * fRadius);	    //z

		    //calculate tangent
		    glm::vec3 tangent;
		    tangent = glm::normalize(glm::vec3((x*zr1 - x*zr_n1), y*zr1 - y*zr_n1, (-z1) - (-z_n1)));

		    puffer.push_back(tangent.x);
		    puffer.push_back(tangent.y);
		    puffer.push_back(tangent.z);

		    //now add texture coordinates

		    puffer.push_back((float)j / longs);	    //z
		    puffer.push_back((float)(i-1) / lats);	    //u
		    pufferInd.push_back(t++);	    //add index

		    /* next longitude */
		    puffer.push_back(x * zr1 * fRadius);
		    puffer.push_back(y * zr1 * fRadius);
		    puffer.push_back(-z1 * fRadius);

		    //calculate tangent
		    tangent = glm::normalize(glm::vec3(x*zr2 - x*zr0, y*zr2 - y*zr0, (-z2) - (-z0)));

		    puffer.push_back(tangent.x);
		    puffer.push_back(tangent.y);
		    puffer.push_back(tangent.z);

		    puffer.push_back((float)(j) / longs);	    //u
		    puffer.push_back((float)(i) / lats);	    //z
		    pufferInd.push_back(t++);
	    }
    }

    GLfloat * pVerticeData = new GLfloat[puffer.size()];
    m_pIndices = new GLuint[puffer.size()];
    /* den Index-Array befüllen. Es wird ein TRIANGLE_STRIP erzeugt, daher im zick-zack verfahren */
    for (a=0,b=0; a < pufferInd.size(); a+=1)
    {
	    m_pIndices[b++] = pufferInd[a];
    }
    /* nun den Vertex-Array befüllen - interleaved mit den Color-Werten und Normalvektoren(x1,y1,z1,r1,g1,b1,Nx1,Ny1,Nz1,x2,y2,z2,r2,g2,b2,Nx2,Ny2,Nz2,...) */
    for (a=0,b=0; a < puffer.size(); a++)
    {
	    pVerticeData[b++] = puffer[a];
    }

    glGenVertexArrays(1, &m_nVertexArrayObject);
    glBindVertexArray(m_nVertexArrayObject);

    /* Vertexbuffer generieren */
    glGenBuffers(1,&m_nVertexBufferObject);
    glGenBuffers(1,&m_nIndexBufferObject);

    /* zum befüllen mal aktivieren ...*/
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
    /* befüllen ... */
    glBufferData(GL_ARRAY_BUFFER, puffer.size()*sizeof(pVerticeData[0]), pVerticeData, GL_STATIC_DRAW);

    /* nun nur noch den Index-Array in den Bufferbereich schreiben */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pufferInd.size()*sizeof(m_pIndices[0]), m_pIndices, GL_STATIC_DRAW);

    m_iNumIndices = pufferInd.size();
    m_iNumVertices = puffer.size() / 8;

    m_pVertices = new GLfloat[(puffer.size() / 8) * 3];

    //create array with vertex positions only
    int iCurrentPos=0;
    for (int i=0; i < puffer.size(); i+=8)
    {
	m_pVertices[iCurrentPos++] = pVerticeData[i];
	m_pVertices[iCurrentPos++] = pVerticeData[i+1];
	m_pVertices[iCurrentPos++] = pVerticeData[i+2];
    }

    delete[] pVerticeData;


    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "SceneObject_Sphere created" << Logger::endl;
}

SceneObject_Sphere::~SceneObject_Sphere()
{
    delete[] m_pVertices;
    delete[] m_pIndices;
}

void SceneObject_Sphere::ItlPreRender()
{
    glBindVertexArray(m_nVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);

    ShaderManager::instance()->pushActiveShader();
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->activateShader("simple_shader");
    else if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1)
	ShaderManager::instance()->activateShader("assimp_deep_step1_shader");
    else if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2)
	ShaderManager::instance()->activateShader("assimp_deep_step2_shader");
    else
	ShaderManager::instance()->activateShader("phong_shader");
}

void SceneObject_Sphere::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();
}

void SceneObject_Sphere::ItlRender()
{
    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));
    const GLint l_normals(ShaderManager::instance()->getAttribute("in_Normal"));
    const GLint l_texcoords(ShaderManager::instance()->getAttribute("in_Texcoord"));

    if (l_in_Position != -1)
    {
	glVertexAttribPointer(l_in_Position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(l_in_Position);
    }

    if (l_normals != -1)
    {
	glVertexAttribPointer(l_normals, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(l_normals);
    }

    if (l_texcoords != -1)
    {
	glVertexAttribPointer(l_texcoords, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(l_texcoords);
    }

    GLint l_color_texture = ShaderManager::instance()->getUniform("color_texture");

    if (l_color_texture != -1)
    {
	GLuint used_texture_unit1 = TextureManager::instance()->useTexture(m_szTextureName);	    //load texture in texture unit
	glUniform1i(l_color_texture,used_texture_unit1);   //tell the shader which texture unit was used
    }

   /* GLint l_shadow_texture = ShaderManager::instance()->getUniform("shadowmap");

    if (l_shadow_texture != -1)
    {
	    GLuint texture_unit_shadowmap = TextureManager::instance()->useTexture("fbo_light_color");	    //load texture in texture unit
	    glUniform1i(l_shadow_texture, texture_unit_shadowmap);   //tell the shader which texture unit was used
    }*/


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObject);
    glDrawElements(GL_TRIANGLE_STRIP, m_iNumIndices, GL_UNSIGNED_INT, NULL);

    if (l_color_texture != -1)
	TextureManager::instance()->unuseTexture(m_szTextureName);				    //tell the texturemanager to release the used texture unit

    //if (l_shadow_texture != -1)
	//TextureManager::instance()->unuseTexture("fbo_light_color");	    //load texture in texture unit
}

float *SceneObject_Sphere::GetVertices()
{
    assert (m_pVertices != NULL);

    return m_pVertices;
}

int *SceneObject_Sphere::GetIndices()
{
    assert(m_pIndices != NULL);
    assert(!"not correct implemented yet - type cast!");

    return (int *)m_pIndices;
}

int SceneObject_Sphere::NumIndices()
{
    return m_iNumIndices;
}

int SceneObject_Sphere::NumVertices()
{
    return m_iNumVertices;
}

bool SceneObject_Sphere::ItlTestSkipRendering()
{
    bool bShowSphere;
    Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bShowSphere", false, bShowSphere);

    return !bShowSphere;
}
