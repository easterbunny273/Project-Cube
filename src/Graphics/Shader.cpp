/*
 * source file for Shadder class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "Graphics/common_gl.h"
#include "Graphics/Shader.h"
#include "Logger.h"


using namespace std;

int last_active;

//#define CHECK_FOR_GLERROR

bool Shader::ItlLoadFileToString(const char* filename, GLubyte** ShaderSource, unsigned long* len)
{
    ifstream file;
    file.open(filename, ios::in); // opens as ASCII!

    if(!file || !file.good())
    {
	Logger::fatal() << "Could not read shader source code from " << filename << Logger::endl;

	return false;
    }

    //unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    *len = file.tellg();
    file.seekg(ios::beg);

    if (*len==0)
    {
	Logger::fatal() << "Could not read shader source code from " << filename << " (File was empty)" << Logger::endl;
	return false;
    }

    *ShaderSource = (GLubyte*) new GLubyte[(*len)+1];

    if (*ShaderSource == 0)
    {
	Logger::fatal() << "Could not reserve memory for shader source code (reading " << filename << ")" << Logger::endl;
	return false;
    }

    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
    (*ShaderSource)[*len] = 0;

    unsigned int i=0;
    while (file.good())
    {
       (*ShaderSource)[i] = file.get();       // get character from file.
       if (!file.eof())
	i++;
    }

    (*ShaderSource)[i] = 0;  // 0-terminate it at the correct position

    file.close();

    return true;
}

void Shader::ItlPrintShaderLog(GLuint obj)
{
    int infologLength = 0;
    int maxLength=0;

    if(glIsShader(obj))
	    glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
    else
	    glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

    char *infoLog = new char[maxLength];

    if (glIsShader(obj))
	    glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
    else
	    glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

    if (infologLength > 1)
	Logger::error() << "Shader messages: \n" << infoLog << Logger::endl;

    delete[] infoLog;
}


void Shader::ItlAddShader(GLenum tShaderType, const char *szFilename)
{
    Logger::debug() << "Loading shader source: " << szFilename << Logger::endl;

    GLubyte *shaderSource = 0;
    unsigned long int shaderLength;

    bool source_ok = false;

    source_ok = ItlLoadFileToString(szFilename, &shaderSource, &shaderLength);

    assert (source_ok);

    GLuint shaderObject = glCreateShader(tShaderType);

    glShaderSource(shaderObject, 1,  (const GLchar **) &shaderSource, (const GLint *) &shaderLength);

    glCompileShader(shaderObject);

    //lets see if an error happened
    GLint compiled;

    //check vertex shader object
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
	ItlPrintShaderLog(shaderObject);
	Logger::fatal() << "Could not compile shader " << szFilename << Logger::endl;
    }
    else
    {
	Logger::debug() << "Successfully compiled shader " << szFilename << Logger::endl;
	m_glShaderObjects.push_back(shaderObject);
    }

    if (shaderSource != 0)
	delete[] shaderSource;
}

void Shader::ItlLinkShader()
{
    GLint ProgramObject = glCreateProgram();

    for (unsigned int i=0; i < m_glShaderObjects.size(); i++)
	glAttachShader(ProgramObject, m_glShaderObjects.at(i));

    glLinkProgram(ProgramObject);

    GLint linked;
    glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);

    if (!linked)
    {
       ItlPrintShaderLog(ProgramObject);
       Logger::fatal() << "Could not link shader program" << Logger::endl;
    }
    else
    {
	    this->m_nShaderId = ProgramObject;
	    this->m_bReadyForUse = true;
	    Logger::debug() << "Successfully linked shader program" << Logger::endl;
    }
}

Shader::Shader(const char *vertexshader_filename, const char *tesselation_control_shader_filename, const char *tesselation_evaluation_shader_filename, const char *geometry_shader_filename, const char *fragmentshader_filename)
{
    ItlAddShader(GL_VERTEX_SHADER, vertexshader_filename);
    ItlAddShader(GL_TESS_CONTROL_SHADER, tesselation_control_shader_filename);
    ItlAddShader(GL_TESS_EVALUATION_SHADER, tesselation_evaluation_shader_filename);
    ItlAddShader(GL_GEOMETRY_SHADER, geometry_shader_filename);
    ItlAddShader(GL_FRAGMENT_SHADER, fragmentshader_filename);
    ItlLinkShader();
}

Shader::Shader(const char *vertexshader_filename, const char *tesselation_control_shader_filename, const char *tesselation_evaluation_shader_filename, const char *fragmentshader_filename)
{
    ItlAddShader(GL_VERTEX_SHADER, vertexshader_filename);
    ItlAddShader(GL_TESS_CONTROL_SHADER, tesselation_control_shader_filename);
    ItlAddShader(GL_TESS_EVALUATION_SHADER, tesselation_evaluation_shader_filename);
    ItlAddShader(GL_FRAGMENT_SHADER, fragmentshader_filename);
    ItlLinkShader();
}

Shader::Shader(const char *vertexshader_filename, const char *fragmentshader_filename) : m_bReadyForUse(false)
{
    ItlAddShader(GL_VERTEX_SHADER, vertexshader_filename);
    ItlAddShader(GL_FRAGMENT_SHADER, fragmentshader_filename);
    ItlLinkShader();
}

Shader::Shader(const char *vertexshader_filename, const char *geometryshader_filename, const char *fragmentshader_filename) : m_bReadyForUse(false)
{
    ItlAddShader(GL_VERTEX_SHADER, vertexshader_filename);
    ItlAddShader(GL_GEOMETRY_SHADER, geometryshader_filename);
    ItlAddShader(GL_FRAGMENT_SHADER, fragmentshader_filename);
    ItlLinkShader();
}


Shader::~Shader()
{
    //delete shader program
    glDeleteProgram(m_nShaderId);

    //delete shader parts
    for_each(m_glShaderObjects.begin(), m_glShaderObjects.end(), [](GLuint shader) { glDeleteShader(shader); });
}

void Shader::activate()
{
    //activate shader if it is ready for use, else log an error message

    if (this->m_bReadyForUse)
    {
#ifdef CHECK_FOR_GLERROR
	glGetError();
#endif

	glUseProgram(this->m_nShaderId);

#ifdef CHECK_FOR_GLERROR
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	    Logger::error() << "glGetError(): " << translateGLerror(error) << Logger::endl;
#endif

	last_active = this->m_nShaderId;
    }
    else
	Logger::error() << "Could not activate shader because it is not ready for use" << Logger::endl;
}

GLint Shader::getUniformLocation(const char *name)
{
    std::string sName(name);

    if ((GLuint) last_active != this->m_nShaderId)
	Logger::error() << "While setting an uniform of an shader, the shader must be active!" << Logger::endl;

    std::map<std::string, GLint>::iterator iter = m_mCachedUniformLocations.find(sName);

    GLint location;

    if (iter == m_mCachedUniformLocations.end())
    {
	location = glGetUniformLocation(this->m_nShaderId, name);
	m_mCachedUniformLocations[sName] = location;
    }
    else
    {
	location = iter->second;
    }

    return location;
}

GLint Shader::getAttribLocation(const char *name)
{
    if (static_cast<GLuint>(last_active) != this->m_nShaderId)
	Logger::error() << "While setting an attribute of an shader, the shader must be active!" << Logger::endl;

    std::string sName(name);

    std::map<std::string, GLint>::iterator iter = m_mCachedAttributeLocations.find(sName);

    GLint location;

    if (iter == m_mCachedAttributeLocations.end())
    {
	location = glGetAttribLocation(this->m_nShaderId, name);
	m_mCachedAttributeLocations[sName] = location;
    }
    else
    {
	location = iter->second;
    }

    return location;
}
