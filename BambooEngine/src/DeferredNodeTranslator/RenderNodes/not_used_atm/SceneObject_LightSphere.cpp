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
#include "Graphics/SceneObjects/SceneObject_LightSphere.h"

using namespace std;

const double PI = 3.141516;

SceneObject_LightSphere::SceneObject_LightSphere() : SceneObject()
{
    int i, j;
    int t=0;	//for index array
    int a, b;
    const int lats=5;
    const int longs=5;
    vector<GLdouble> puffer;
    vector<GLint> pufferInd;
    GLdouble *vertexArray;
    GLuint *indexArray;

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

		    puffer.push_back(x*zr0);	    //x
		    puffer.push_back(y*zr0);	    //y
		    puffer.push_back(-z0);	    //z

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
		    puffer.push_back(x*zr1);
		    puffer.push_back(y*zr1);
		    puffer.push_back(-z1);

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
    vertexArray = new GLdouble[puffer.size()];
    indexArray = new GLuint[puffer.size()];
    /* den Index-Array befüllen. Es wird ein TRIANGLE_STRIP erzeugt, daher im zick-zack verfahren */
    for (a=0,b=0; a < pufferInd.size(); a+=1)
    {
	    indexArray[b++] = pufferInd[a];
    }
    /* nun den Vertex-Array befüllen - interleaved mit den Color-Werten und Normalvektoren(x1,y1,z1,r1,g1,b1,Nx1,Ny1,Nz1,x2,y2,z2,r2,g2,b2,Nx2,Ny2,Nz2,...) */
    for (a=0,b=0; a < puffer.size(); a++)
    {
	    vertexArray[b++] = puffer[a];
    }

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    /* Vertexbuffer generieren */
    glGenBuffers(1,&this->vertex_buffer);
    glGenBuffers(1,&this->index_buffer);
    /* zum befüllen mal aktivieren ...*/
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    /* befüllen ... */
    glBufferData(GL_ARRAY_BUFFER,puffer.size()*sizeof(vertexArray[0]), vertexArray, GL_STATIC_DRAW);

    /* nun nur noch den Index-Array in den Bufferbereich schreiben */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pufferInd.size()*sizeof(indexArray[0]), indexArray, GL_STATIC_DRAW);

    this->indexArray_size = pufferInd.size();

    delete[] vertexArray;
    delete[] indexArray;

    //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "SceneObject_LightSphere created" << Logger::endl;
}

SceneObject_LightSphere::~SceneObject_LightSphere()
{

}

void SceneObject_LightSphere::ItlPreRenderChildren()
{

}

void SceneObject_LightSphere::ItlPostRenderChildren()
{

}

void SceneObject_LightSphere::ItlPreRender()
{
    ShaderManager::instance()->pushActiveShader();
    ShaderManager::instance()->activateShader("light-sphere");
}

void SceneObject_LightSphere::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();
}

void SceneObject_LightSphere::ItlRender()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->index_buffer);

    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));

    glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(GLdouble), NULL);
    glEnableVertexAttribArray(l_in_Position);

    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLE_STRIP, this->indexArray_size, GL_UNSIGNED_INT, NULL);
    glEnable(GL_DEPTH_TEST);
}
