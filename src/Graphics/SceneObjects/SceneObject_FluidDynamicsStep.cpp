/*
 * source file for SceneObject_FluidDynamicsStep class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 06/2011
 */

#include <string>
#include <assert.h>

#include "Logger.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/SceneObjects/SceneObject_FluidDynamicsStep.h"

const int iGridSize = 256;
const float fGridSize = (float) iGridSize;
const float fReGridSize = 1 / fGridSize;
const float fHalfReGridSize = 2 / fGridSize;

SceneObject_FluidDynamicsStep::SceneObject_FluidDynamicsStep(std::string sInteriorShader, float fBoundaryScale)
    :	m_sInteriorShader(sInteriorShader),
	m_fBoundaryScale(fBoundaryScale),
	m_bBoundary(true),
	m_vBoundaryOffset()
{
    const GLdouble vertices3f[]= {
      -1.0, 1.0, 0.0,    //vertex position
      0.0, iGridSize,	0.0,    //texcoords

     -1.0, -1.0, 0.0,	    //vertex position
     0.0, 0.0, 0.0,	    //texcoords

      1.0, 1.0, 0.0,	    //vertex position
      iGridSize, iGridSize, 0.0,	    //texcoords

      1.0, -1.0, 0.0,
      iGridSize, 0.0, 0.0,	    //texcoords

    //now vertices for the lines

	-1.0 + fHalfReGridSize , -1.0 + fHalfReGridSize, 0.0,    //vertex position
	0.0, 0.0,	0.0,    //texcoords

       -1.0 + fHalfReGridSize, 1.0, 0.0,	    //vertex position
       0.0, iGridSize, 0.0,	    //texcoords

	1.0, 1.0, 0.0,	    //vertex position
	iGridSize, iGridSize, 0.0,	    //texcoords

	1.0, -1.0 + fHalfReGridSize, 0.0,
	iGridSize, 0.0, 0.0,	    //texcoords

	-1.0 - fHalfReGridSize , -1.0 + fHalfReGridSize, 0.0,    //vertex position
	0.0, 0.0,	0.0,    //texcoords
	};

    //clear error status
    glGetError();

    // create VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    //in vec3 in_Position;
    glGenBuffers(1, &this->buffer_vertices3f);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3f), vertices3f, GL_STATIC_DRAW);

     //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "SceneObject_FluidDynamicsStep created" << Logger::endl;
}

SceneObject_FluidDynamicsStep::SceneObject_FluidDynamicsStep(std::string sInteriorShader)
    :	m_sInteriorShader(sInteriorShader),
	m_fBoundaryScale(0.0f),
	m_bBoundary(false),
	m_vBoundaryOffset()
{
    const GLdouble vertices3f[]= {
      -1.0, 1.0, 0.0,    //vertex position
      0.0, iGridSize,	0.0,    //texcoords

     -1.0, -1.0, 0.0,	    //vertex position
     0.0, 0.0, 0.0,	    //texcoords

      1.0, 1.0, 0.0,	    //vertex position
      iGridSize, iGridSize, 0.0,	    //texcoords

      1.0, -1.0, 0.0,
      iGridSize, 0.0, 0.0,	    //texcoords

    //now vertices for the lines

    -1.0 + fHalfReGridSize , -1.0 + fHalfReGridSize, 0.0,    //vertex position
    0.0, 0.0,	0.0,    //texcoords

   -1.0 + fHalfReGridSize, 1.0 + fHalfReGridSize, 0.0,	    //vertex position
   0.0, iGridSize, 0.0,	    //texcoords

    1.0 + fHalfReGridSize, 1.0 + fHalfReGridSize, 0.0,	    //vertex position
    iGridSize, iGridSize, 0.0,	    //texcoords

    1.0 + fHalfReGridSize, -1.0 + fHalfReGridSize, 0.0,
    iGridSize, 0.0, 0.0,	    //texcoords

    -1.0 + fHalfReGridSize , -1.0 + fHalfReGridSize, 0.0,    //vertex position
    0.0, 0.0,	0.0,    //texcoords
    };

    //clear error status
    glGetError();

    // create VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    //in vec3 in_Position;
    glGenBuffers(1, &this->buffer_vertices3f);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3f), vertices3f, GL_STATIC_DRAW);

     //check for error
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
        Logger::error() << "glGetError: " << TranslateGLerror(error) << Logger::endl;

    Logger::debug() << "SceneObject_FluidDynamicsStep created" << Logger::endl;
}

SceneObject_FluidDynamicsStep::~SceneObject_FluidDynamicsStep()
{

}

void SceneObject_FluidDynamicsStep::ItlPreRender()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);

    ShaderManager::instance()->pushActiveShader();
    ShaderManager::instance()->activateShader(m_sInteriorShader);

    // load all uniform samplers in units
    for (auto iter=m_mNecessaryTextures.begin(); iter != m_mNecessaryTextures.end(); iter++)
    {
	std::string sTexture = iter->first;

	m_mNecessaryTextures[sTexture] = TextureManager::instance()->useTexture(sTexture);

	assert (m_mNecessaryTextures[sTexture] != -1);
    }
}

void SceneObject_FluidDynamicsStep::ItlRender()
{
    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));
    const GLint l_texcoords(ShaderManager::instance()->getAttribute("in_Texcoord"));

    assert (l_in_Position != -1);
    assert (l_texcoords != -1);

    glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
    glVertexAttribPointer(l_texcoords, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid *) (3 * sizeof(GLdouble)));

    glEnableVertexAttribArray(l_in_Position);
    glEnableVertexAttribArray(l_texcoords);

    ItlSetUserDefinedUniforms();

    //draw data
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(l_in_Position);
    glDisableVertexAttribArray(l_texcoords);

    /* now render boundary */
    if (m_bBoundary)
    {
	glClear(GL_DEPTH_BUFFER_BIT);
	ShaderManager::instance()->activateShader("fluid-dynamics-boundary");

	const GLint l_in_Position2(ShaderManager::instance()->getAttribute("in_Position"));
	const GLint l_texcoords2(ShaderManager::instance()->getAttribute("in_Texcoord"));

	assert (l_in_Position2 != -1);
	assert (l_texcoords2 != -1);

	glLineWidth(1.0f);

	glVertexAttribPointer(l_in_Position2, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
	glVertexAttribPointer(l_texcoords2, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid *) (3 * sizeof(GLdouble)));

	glEnableVertexAttribArray(l_in_Position2);
	glEnableVertexAttribArray(l_texcoords2);



	GLint l_offset_x = ShaderManager::instance()->getUniform("offset_x");
	GLint l_offset_y = ShaderManager::instance()->getUniform("offset_y");
	GLint l_scale = ShaderManager::instance()->getUniform("scale");
	GLint l_texture1 = ShaderManager::instance()->getUniform("texture1");

	assert (l_offset_x != -1);
	assert (l_offset_y != -1);
	assert (l_scale != -1);
	assert (l_texture1 != -1);

	GLuint nUsedTextureUnit = m_mNecessaryTextures.begin()->second;

	glUniform1i(l_texture1, nUsedTextureUnit);

	glUniform1f(l_scale, m_fBoundaryScale);

	glUniform1f(l_offset_x, 1.0f);
	glUniform1f(l_offset_y, 0.0f);

	glDrawArrays(GL_LINES, 4, 2);

	glUniform1f(l_offset_x, 0.0f);
	glUniform1f(l_offset_y, -1.0f);
	glDrawArrays(GL_LINES, 5, 2);

	glUniform1f(l_offset_x, -1.0f);
	glUniform1f(l_offset_y, 0.0f);
	glDrawArrays(GL_LINES, 6, 2);

	glUniform1f(l_offset_x, 0.0f);
	glUniform1f(l_offset_y, 1.0f);
	glDrawArrays(GL_LINES, 7, 2);


    }

    //glDrawArrays(GL_LINE_LOOP, 4, 4);

}

void SceneObject_FluidDynamicsStep::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();

    // unload all uniform samplers in units
    for (auto iter=m_mNecessaryTextures.begin(); iter != m_mNecessaryTextures.end(); iter++)
    {
	std::string sTexture = iter->first;

	assert (m_mNecessaryTextures[sTexture] != -1);

	TextureManager::instance()->unuseTexture(sTexture);

	m_mNecessaryTextures[sTexture] = -1;
    }
}

void SceneObject_FluidDynamicsStep::SetUniform(std::string sUniform, float fValue)
{
    m_mUniforms_Floats[sUniform] = fValue;
}

void SceneObject_FluidDynamicsStep::SetUniform(std::string sUniform, int iValue)
{
    m_mUniforms_GLints[sUniform] = iValue;
}

void SceneObject_FluidDynamicsStep::SetUniform(std::string sUniform, glm::vec2 v2Vector)
{
    m_mUniforms_Vec2[sUniform] = v2Vector;
}

void SceneObject_FluidDynamicsStep::SetUniform(std::string sUniform, glm::vec3 v3Vector)
{
    m_mUniforms_Vec3[sUniform] = v3Vector;
}

void SceneObject_FluidDynamicsStep::ItlSetUserDefinedUniforms()
{

    // set the user defined sampler ids (= used texture units)
    for (auto iter=m_mUniforms_Samplers.begin(); iter != m_mUniforms_Samplers.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->getUniform(iter->first);
	GLint iUsedUnit = m_mNecessaryTextures[iter->second];

	assert (iUsedUnit != -1);

	GLuint nUsedUnit = (GLuint) iUsedUnit;

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	glUniform1i(ShaderManager::instance()->getUniform(iter->first), nUsedUnit);
    }

    // set the user defined uniforms
    for (auto iter=m_mUniforms_GLints.begin(); iter != m_mUniforms_GLints.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->getUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform1i(ShaderManager::instance()->getUniform(iter->first), iter->second);
    }

    // set the user defined uniforms
    for (auto iter=m_mUniforms_Floats.begin(); iter != m_mUniforms_Floats.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->getUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform1f(ShaderManager::instance()->getUniform(iter->first), iter->second);
    }

    for (auto iter=m_mUniforms_Vec2.begin(); iter != m_mUniforms_Vec2.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->getUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform2f(ShaderManager::instance()->getUniform(iter->first), iter->second.x, iter->second.y);
    }

    for (auto iter=m_mUniforms_Vec3.begin(); iter != m_mUniforms_Vec3.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->getUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform3f(ShaderManager::instance()->getUniform(iter->first), iter->second.x, iter->second.y, iter->second.z);
    }
}

void SceneObject_FluidDynamicsStep::SetUniformSampler(std::string sSamplerName, std::string sTextureName)
{
    int iSize = m_mUniforms_Samplers.size();

    m_mUniforms_Samplers[sSamplerName] = sTextureName;
    m_mNecessaryTextures[sTextureName] = -1;

    assert (iSize == m_mUniforms_Samplers.size() - 1);
}
