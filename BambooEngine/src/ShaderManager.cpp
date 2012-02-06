#include <string>
#include <algorithm>

#include "ShaderManager.h"
#include "PC_Logger.h"

using namespace std;

Bamboo::ShaderManager::ShaderManager()
{
    //nothing to do so far
    m_nCurrentActiveShaderProgram = 0;
}

Bamboo::ShaderManager::~ShaderManager()
{
    // destroy all used Shader instances
    // C++11 construct with lambda function :)
    for_each(m_vpShaders.begin(), m_vpShaders.end(), [](Shader *pShader) { delete pShader; });
}

void Bamboo::ShaderManager::AddShader(std::string sName, Shader *pShader)
{
    m_vpShaders.push_back(pShader);
    m_vsShaderNames.push_back(sName);
}

bool Bamboo::ShaderManager::ActivateShader(std::string sName)
{
    bool bSuccess = false;

    for (unsigned int a=0; a < m_vpShaders.size(); a++)
    {
	if (m_vsShaderNames[a].compare(sName)==0)
	{
            if (m_nCurrentActiveShaderProgram != a)
            {
                m_vpShaders[a]->Activate();
                m_nCurrentActiveShaderProgram = a;               
            }

            bSuccess = true;
            break;
	}
    }

    if (bSuccess == false)
	Logger::error() << "Could not activate shader with internal name \"" << sName  << "\", because it is not registered in ShaderManager" << Logger::endl;

    return bSuccess;
}

GLint Bamboo::ShaderManager::GetAttribute(std::string sAttributeName)
{
    return m_vpShaders[m_nCurrentActiveShaderProgram]->GetAttribLocation(sAttributeName.data());
}

GLint Bamboo::ShaderManager::GetUniform(std::string sUniformName)
{
    return m_vpShaders[m_nCurrentActiveShaderProgram]->GetUniformLocation(sUniformName.data());
}

void Bamboo::ShaderManager::PushActiveShader()
{
    m_vActiveShaderStack.push(m_nCurrentActiveShaderProgram);
}

void Bamboo::ShaderManager::PopActiveShader()
{
    unsigned int nOldActiveShader = m_nCurrentActiveShaderProgram;

    m_nCurrentActiveShaderProgram = m_vActiveShaderStack.top();

    m_vActiveShaderStack.pop();

    if (m_nCurrentActiveShaderProgram != nOldActiveShader)
        m_vpShaders[m_nCurrentActiveShaderProgram]->Activate();
}
