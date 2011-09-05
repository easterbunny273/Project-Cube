#include <string>
#include <algorithm>

#include "Graphics/ShaderManager.h"
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
    // destroy all used Shader instances
    // C++0x construct with lambda function :)
    for_each(m_vpShaders.begin(), m_vpShaders.end(), [](Shader *pShader) { delete pShader; });
}

void ShaderManager::AddShader(std::string sName, Shader *pShader)
{
    m_vpShaders.push_back(pShader);
    m_vsShaderNames.push_back(sName);
}

bool ShaderManager::ActivateShader(std::string sName)
{
    bool bSuccess = false;

    for (unsigned int a=0; a < m_vpShaders.size(); a++)
    {
	if (m_vsShaderNames[a].compare(sName)==0)
	{
	    m_vpShaders[a]->Activate();
	    m_nCurrentActiveShaderProgram = a;
	    bSuccess = true;
	    break;
	}
    }

    if (bSuccess == false)
	Logger::error() << "Could not activate shader with internal name \"" << sName  << "\", because it is not registered in ShaderManager" << Logger::endl;

    return bSuccess;
}

GLint ShaderManager::GetAttribute(std::string sAttributeName)
{
    return m_vpShaders[m_nCurrentActiveShaderProgram]->GetAttribLocation(sAttributeName.data());
}

GLint ShaderManager::GetUniform(std::string sUniformName)
{
    return m_vpShaders[m_nCurrentActiveShaderProgram]->GetUniformLocation(sUniformName.data());
}

void ShaderManager::PushActiveShader()
{
    m_vActiveShaderStack.push(m_nCurrentActiveShaderProgram);
}

void ShaderManager::PopActiveShader()
{
    m_nCurrentActiveShaderProgram = m_vActiveShaderStack.top();
    m_vActiveShaderStack.pop();
    m_vpShaders[m_nCurrentActiveShaderProgram]->Activate();
}
