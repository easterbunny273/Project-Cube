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

SceneObject_PostEffect::SceneObject_PostEffect(std::string shader_name, std::string texture_name, std::string texture_name2, std::string texture_name3)
    : m_sShaderName(shader_name),
    m_sTextureName(texture_name),
    m_sTextureName2(texture_name2),
    m_sTextureName3(texture_name3)
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

    ShaderManager::instance()->pushActiveShader();
    ShaderManager::instance()->activateShader(m_sShaderName);
}

void SceneObject_PostEffect::ItlRender()
{
    const GLint l_in_Position(ShaderManager::instance()->getAttribute("in_Position"));
    const GLint l_texcoords(ShaderManager::instance()->getAttribute("in_Texcoord"));

    bool bTexture2HasOwnUnit = false;
    bool bTexture3HasOwnUnit = false;

    glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
    glVertexAttribPointer(l_texcoords, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid *) (3 * sizeof(GLdouble)));

    glEnableVertexAttribArray(l_in_Position);
    glEnableVertexAttribArray(l_texcoords);

    //load first texture and tell the shader the units id
    GLuint used_texture_unit1 = TextureManager::instance()->useTexture(m_sTextureName);	    //load texture in texture unit

    glUniform1i(ShaderManager::instance()->getUniform("texture1"),used_texture_unit1);   //tell the shader which texture unit was used

    //if two textures were given, load the second texture in an unit and tell the id the shader
    if (m_sTextureName2.length() > 0)
    {
	assert (ShaderManager::instance()->getUniform("texture2") != -1);

	if (m_sTextureName.compare(m_sTextureName2) == 0)
	    glUniform1i(ShaderManager::instance()->getUniform("texture2"),used_texture_unit1);
	else
	{
	    GLuint used_texture_unit2 = TextureManager::instance()->useTexture(m_sTextureName2);	    //load texture in texture unit
	    glUniform1i(ShaderManager::instance()->getUniform("texture2"),used_texture_unit2);   //tell the shader which texture unit was used

	    bTexture2HasOwnUnit = true;
	}
    }

    //if three textures were given, load the third texture in an unit and tell the id the shader
    if (m_sTextureName3.length() > 0)
    {
	if (m_sTextureName.compare(m_sTextureName3) == 0)
	    glUniform1i(ShaderManager::instance()->getUniform("texture2"),used_texture_unit1);
	else
	{
	    GLuint used_texture_unit3 = TextureManager::instance()->useTexture(m_sTextureName3);	    //load texture in texture unit
	    glUniform1i(ShaderManager::instance()->getUniform("texture3"),used_texture_unit3);   //tell the shader which texture unit was used

	    bTexture3HasOwnUnit = true;
	}

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

    //draw data
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //release used texture units
    TextureManager::instance()->unuseTexture(m_sTextureName);				    //tell the texturemanager to release the used texture unit
    if (m_sTextureName2.length() > 0 && bTexture2HasOwnUnit)
	TextureManager::instance()->unuseTexture(m_sTextureName2);

    if (m_sTextureName3.length() > 0 && bTexture3HasOwnUnit)
	TextureManager::instance()->unuseTexture(m_sTextureName3);
}

void SceneObject_PostEffect::ItlPostRender()
{
    ShaderManager::instance()->popActiveShader();
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
