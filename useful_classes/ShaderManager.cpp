#include <string>
#include <algorithm>

#include "ShaderManager.h"
#include "Logger.h"

using namespace std;

ShaderManager *ShaderManager::instance()
{
    static ShaderManager shadermanager_singleton_instance;

    return &shadermanager_singleton_instance;
}

ShaderManager::ShaderManager()
{
    //nothing to do so far
}

ShaderManager::~ShaderManager()
{
    //destroy all used Shader instances
    for_each(m_vpShaders.begin(), m_vpShaders.end(), [](Shader *pShader) { delete pShader; });
}

void ShaderManager::addShader(std::string name, Shader *shader)
{
    this->m_vpShaders.push_back(shader);
    this->m_vsShaderNames.push_back(name);
}

bool ShaderManager::activateShader(std::string name)
{
    int a;
    bool success = false;

    for (a=0; a < this->m_vpShaders.size(); a++)
    {
	if (this->m_vsShaderNames[a].compare(name)==0)
	{
	    this->m_vpShaders[a]->activate();
	    this->m_nCurrentActiveShaderProgram = a;
	    success = true;
	    break;
	}
    }

    if (success == false)
	Logger::error() << "Could not activate shader with internal name \"" << name  << "\", because it is not registered in ShaderManager" << Logger::endl;

    return success;
}

GLint ShaderManager::getAttribute(std::string attribute_name)
{
    return this->m_vpShaders[this->m_nCurrentActiveShaderProgram]->getAttribLocation(attribute_name.data());
}

GLint ShaderManager::getUniform(std::string uniform_name)
{
    return this->m_vpShaders[this->m_nCurrentActiveShaderProgram]->getUniformLocation(uniform_name.data());
}

void ShaderManager::pushActiveShader()
{
    m_vActiveShaderStack.push(this->m_nCurrentActiveShaderProgram);
}

void ShaderManager::popActiveShader()
{
    this->m_nCurrentActiveShaderProgram = this->m_vActiveShaderStack.top();
    this->m_vActiveShaderStack.pop();
    this->m_vpShaders[this->m_nCurrentActiveShaderProgram]->activate();
}
