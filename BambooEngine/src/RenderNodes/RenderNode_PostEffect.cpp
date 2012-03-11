/*
 * source file for SceneObject_PostEffect class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2010
 */

#include <string>

#include "TextureManager.h"
#include "PC_Logger.h"
#include "ShaderManager.h"
#include "RenderNodes/RenderNode_PostEffect.h"

Bamboo::RN_PostEffect::RN_PostEffect(std::string sShaderToUse)
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

    // prepare the vertex array object
    ItlPrepareVAO();

    Logger::debug() << "RN_PostEffect created" << Logger::endl;
}

Bamboo::RN_PostEffect::~RN_PostEffect()
{
  Logger::debug() << "RN_PostEffect destroyed" << Logger::endl;
}

void Bamboo::RN_PostEffect::ItlPreRender()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);

    ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
    ItlGetGraphicCore()->GetShaderManager()->ActivateShader(m_sShaderName);
}

void Bamboo::RN_PostEffect::ItlRender()
{
    // get texture manager
    Bamboo::TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    std::vector <GLuint> vManualLockedUnits;

    // load necessary textures and tell the shader
    for (auto iter = m_mTextures.begin(); iter != m_mTextures.end(); iter++)
    {
        // alias for better code reading
        std::string sTextureName = iter->second;
        std::string sUniformName = iter->first;

        // load texture in a free unit and remember used texture unit
        GLuint nUsedTextureUnit = pTextureManager->UseTexture(sTextureName);

        // get the uniform location
        GLint iUniformLocation = ItlGetGraphicCore()->GetShaderManager()->GetUniform(sUniformName);

        // check if the uniform exists
        assert (iUniformLocation != -1);

        // write used texture unit to shader uniform
        glUniform1i(iUniformLocation, nUsedTextureUnit);
    }

    // load necessary textures and tell the shader
    for (auto iter = m_mTexturesDirect.begin(); iter != m_mTexturesDirect.end(); iter++)
    {
        // alias for better code reading
        GLuint nTextureID = iter->second;
        std::string sUniformName = iter->first;

        // load texture in a free unit and remember used texture unit
        GLuint nUsedTextureUnit = pTextureManager->RequestFreeUnit();

        // get the uniform location
        GLint iUniformLocation = ItlGetGraphicCore()->GetShaderManager()->GetUniform(sUniformName);

        // check if the uniform exists
        assert (iUniformLocation != -1);

        glActiveTexture(GL_TEXTURE0 + nUsedTextureUnit);
        glBindTexture(GL_TEXTURE_2D, nTextureID);

        // write used texture unit to shader uniform
        glUniform1i(iUniformLocation, nUsedTextureUnit);

        vManualLockedUnits.push_back(nUsedTextureUnit);
    }


    // set the user defined uniforms
    for (auto iter=m_mUniforms_Floats.begin(); iter != m_mUniforms_Floats.end(); iter++)
    {
        GLint iLocation = ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
            glUniform1f(ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first), iter->second);
    }

    for (auto iter=m_mUniforms_Vec2.begin(); iter != m_mUniforms_Vec2.end(); iter++)
    {
        GLint iLocation = ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
            glUniform2f(ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first), iter->second.x, iter->second.y);
    }

    for (auto iter=m_mUniforms_Vec3.begin(); iter != m_mUniforms_Vec3.end(); iter++)
    {
        GLint iLocation = ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first);

	std::string sDebug = iter->first;

	assert (iLocation != -1);

	if (iLocation != -1)
            glUniform3f(ItlGetGraphicCore()->GetShaderManager()->GetUniform(iter->first), iter->second.x, iter->second.y, iter->second.z);
    }

    //draw data
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //release used texture units
    for (auto iter = m_mTextures.begin(); iter != m_mTextures.end(); iter++)
    {
        // alias for better code reading
        std::string sTextureName = iter->second;

        pTextureManager->UnuseTexture(sTextureName);
    }

    //release used texture units
    for (auto iter = vManualLockedUnits.begin(); iter != vManualLockedUnits.end(); iter++)
    {
        pTextureManager->ReleaseUnit(*iter);
    }
}

void Bamboo::RN_PostEffect::ItlPostRender()
{
  ItlGetGraphicCore()->GetShaderManager()->PopActiveShader();
}

void Bamboo::RN_PostEffect::ItlPrepareVAO()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->buffer_vertices3f);

  ItlGetGraphicCore()->GetShaderManager()->PushActiveShader();
  ItlGetGraphicCore()->GetShaderManager()->ActivateShader(m_sShaderName);

  const GLint l_in_Position(ItlGetGraphicCore()->GetShaderManager()->GetAttribute("in_Position"));
  const GLint l_texcoords(ItlGetGraphicCore()->GetShaderManager()->GetAttribute("in_Texcoord"));

  glVertexAttribPointer(l_in_Position, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), NULL);
  glVertexAttribPointer(l_texcoords, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid *) (3 * sizeof(GLdouble)));

  glEnableVertexAttribArray(l_in_Position);
  glEnableVertexAttribArray(l_texcoords);

  ItlGetGraphicCore()->GetShaderManager()->PopActiveShader();
}

void Bamboo::RN_PostEffect::SetUniform(std::string sUniform, float fValue)
{
    m_mUniforms_Floats[sUniform] = fValue;
}

void Bamboo::RN_PostEffect::SetUniform(std::string sUniform, glm::vec2 v2Vector)
{
    m_mUniforms_Vec2[sUniform] = v2Vector;
}

void Bamboo::RN_PostEffect::SetUniform(std::string sUniform, glm::vec3 v3Vector)
{
    m_mUniforms_Vec3[sUniform] = v3Vector;
}

void Bamboo::RN_PostEffect::SetTexture(std::string sUniformName, std::string sTextureName)
{
    m_mTextures[sUniformName] = sTextureName;
}

void Bamboo::RN_PostEffect::SetTexture(std::string sUniformName, GLuint nTextureID)
{
    m_mTexturesDirect[sUniformName] = nTextureID;
}

