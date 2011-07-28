/*
 * source file for SceneObject_PostEffect class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#include <string>

#include "Graphics/TextureManager.h"
#include "Logger.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/SceneObjects/SceneObject_PostEffect.h"

SceneObject_PostEffect::SceneObject_PostEffect(std::string sShaderToUse)
    : m_sShaderName(sShaderToUse)
{
    const GLdouble vertices3f[]= {
      -1.0, 1.0, 0.0,    //vertex position
      0.0, 1.0,	0.0,    //texcoords

     -1.0, -1.0, 0.0,	    //vertex position
     0.0, 0.0, 0.0,	    //texcoords

      1.0, 1.0, 0.0,	    //vertex position
      1.0, 1.0, 0.0,	    //texcoords

      1.0, -1.0, 0.0,
      1.0, 0.0, 0.0,	    //texcoords
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

    Logger::debug() << "SceneObject_PostEffect created" << Logger::endl;
}

SceneObject_PostEffect::~SceneObject_PostEffect()
{

}

void SceneObject_PostEffect::ItlPreRender()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);

    ShaderManager::instance()->PushActiveShader();
    ShaderManager::instance()->ActivateShader(m_sShaderName);
}

void SceneObject_PostEffect::ItlRender()
{
    const GLint l_in_Position(ShaderManager::instance()->GetAttribute("in_Position"));
    const GLint l_texcoords(ShaderManager::instance()->GetAttribute("in_Texcoord"));

    glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
    glVertexAttribPointer(l_texcoords, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid *) (3 * sizeof(GLdouble)));

    glEnableVertexAttribArray(l_in_Position);
    glEnableVertexAttribArray(l_texcoords);

    // load necessary textures and tell the shader
    for (auto iter = m_mTextures.begin(); iter != m_mTextures.end(); iter++)
    {
        // alias for better code reading
        std::string sTextureName = iter->second;
        std::string sUniformName = iter->first;

        // load texture in a free unit and remember used texture unit
        GLuint nUsedTextureUnit = TextureManager::instance()->useTexture(sTextureName);

        // get the uniform location
        GLint iUniformLocation = ShaderManager::instance()->GetUniform(sUniformName);

        // check if the uniform exists
        assert (iUniformLocation != -1);

        // write used texture unit to shader uniform
        glUniform1i(iUniformLocation, nUsedTextureUnit);
    }


    // set the user defined uniforms
    for (auto iter=m_mUniforms_Floats.begin(); iter != m_mUniforms_Floats.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform1f(ShaderManager::instance()->GetUniform(iter->first), iter->second);
    }

    for (auto iter=m_mUniforms_Vec2.begin(); iter != m_mUniforms_Vec2.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform2f(ShaderManager::instance()->GetUniform(iter->first), iter->second.x, iter->second.y);
    }

    for (auto iter=m_mUniforms_Vec3.begin(); iter != m_mUniforms_Vec3.end(); iter++)
    {
	GLint iLocation = ShaderManager::instance()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
	    glUniform3f(ShaderManager::instance()->GetUniform(iter->first), iter->second.x, iter->second.y, iter->second.z);
    }

    //draw data
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //release used texture units
    for (auto iter = m_mTextures.begin(); iter != m_mTextures.end(); iter++)
    {
        // alias for better code reading
        std::string sTextureName = iter->second;

        TextureManager::instance()->unuseTexture(sTextureName);
    }
}

void SceneObject_PostEffect::ItlPostRender()
{
    ShaderManager::instance()->PopActiveShader();
}

void SceneObject_PostEffect::SetUniform(std::string sUniform, float fValue)
{
    m_mUniforms_Floats[sUniform] = fValue;
}

void SceneObject_PostEffect::SetUniform(std::string sUniform, glm::vec2 v2Vector)
{
    m_mUniforms_Vec2[sUniform] = v2Vector;
}

void SceneObject_PostEffect::SetUniform(std::string sUniform, glm::vec3 v3Vector)
{
    m_mUniforms_Vec3[sUniform] = v3Vector;
}
