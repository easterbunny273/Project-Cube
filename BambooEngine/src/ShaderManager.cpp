#include <string>
#include <algorithm>

#include "ShaderManager.h"
#include "BambooLib/include/Logger.h"

using namespace BambooLib;

namespace BambooGraphics
{
    /***************************************************************************
     ***************************************************************************/
    ShaderManager::ShaderManager()
    {
        // nothing to do so far
    }

    /***************************************************************************
     ***************************************************************************/
    ShaderManager::~ShaderManager()
    {
        // clean up, delete all compiled shaders and linked programs

        for (auto iter=m_mpShaders.begin(); iter != m_mpShaders.end(); iter++)
        {
            // delete shader objects
            for (auto iter_o = iter->second->vCompiledShaderObjects.begin();
                        iter_o != iter->second->vCompiledShaderObjects.end();
                        iter_o++)
                glDeleteShader(*iter_o);

            // delete shader program object
            glDeleteProgram(iter->second->nGLShaderName);
        }

        m_mpShaders.clear();
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::CreateAndRegisterShader(const std::string sName,
                                                const std::string sVertexShaderFilename,
                                                const std::string sFragmentShaderFilename)
    {
        std::vector<GLuint> vCompiledShaderObjects;

        GLuint iNewCompiledShaderObject;
        bool bOk = ItlLoadAndCompileShader(GL_VERTEX_SHADER, sVertexShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_FRAGMENT_SHADER, sFragmentShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        if (bOk)
        {
            GLuint iNewShaderProgram;
            bOk &= ItlLinkShaderProgram(vCompiledShaderObjects, iNewShaderProgram);

            if (bOk)
            {
                // create new shader program
                TItlShader *pNewShaderProgram = new TItlShader(iNewShaderProgram, vCompiledShaderObjects);

                // check that shader program with that name is not registered yet
                assert (m_mpShaders.find(sName) == m_mpShaders.end());

                // insert into map
                m_mpShaders[sName] = pNewShaderProgram;
            }
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::CreateAndRegisterShader(const std::string sName,
                                                const std::string sVertexShaderFilename,
                                                const std::string sGeometryShaderFilename,
                                                const std::string sFragmentShaderFilename)
    {
        std::vector<GLuint> vCompiledShaderObjects;

        GLuint iNewCompiledShaderObject;
        bool bOk = ItlLoadAndCompileShader(GL_VERTEX_SHADER, sVertexShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_GEOMETRY_SHADER, sGeometryShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_FRAGMENT_SHADER, sFragmentShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        if (bOk)
        {
            GLuint iNewShaderProgram;
            bOk &= ItlLinkShaderProgram(vCompiledShaderObjects, iNewShaderProgram);

            if (bOk)
            {
                // create new shader program
                TItlShader *pNewShaderProgram = new TItlShader(iNewShaderProgram, vCompiledShaderObjects);

                // check that shader program with that name is not registered yet
                assert (m_mpShaders.find(sName) == m_mpShaders.end());

                // insert into map
                m_mpShaders[sName] = pNewShaderProgram;
            }
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::CreateAndRegisterShader(const std::string sName,
                                                const std::string sVertexShaderFilename,
                                                const std::string sTesselationControlShaderFilename,
                                                const std::string sTesselationEvaluationShaderFilename,
                                                const std::string sFragmentShaderFilename)
    {
        std::vector<GLuint> vCompiledShaderObjects;

        GLuint iNewCompiledShaderObject;
        bool bOk = ItlLoadAndCompileShader(GL_VERTEX_SHADER, sVertexShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_TESS_CONTROL_SHADER, sTesselationControlShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_TESS_EVALUATION_SHADER, sTesselationEvaluationShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_FRAGMENT_SHADER, sFragmentShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        if (bOk)
        {
            GLuint iNewShaderProgram;
            bOk &= ItlLinkShaderProgram(vCompiledShaderObjects, iNewShaderProgram);

            if (bOk)
            {
                // create new shader program
                TItlShader *pNewShaderProgram = new TItlShader(iNewShaderProgram, vCompiledShaderObjects);

                // check that shader program with that name is not registered yet
                assert (m_mpShaders.find(sName) == m_mpShaders.end());

                // insert into map
                m_mpShaders[sName] = pNewShaderProgram;
            }
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::CreateAndRegisterShader(const std::string sName,
                                                const std::string sVertexShaderFilename,
                                                const std::string sTesselationControlShaderFilename,
                                                const std::string sTesselationEvaluationShaderFilename,
                                                const std::string sGeometryShaderFilename,
                                                const std::string sFragmentShaderFilename)
    {
        std::vector<GLuint> vCompiledShaderObjects;

        GLuint iNewCompiledShaderObject;
        bool bOk = ItlLoadAndCompileShader(GL_VERTEX_SHADER, sVertexShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_TESS_CONTROL_SHADER, sTesselationControlShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_TESS_EVALUATION_SHADER, sTesselationEvaluationShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_GEOMETRY_SHADER, sGeometryShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        bOk &= ItlLoadAndCompileShader(GL_FRAGMENT_SHADER, sFragmentShaderFilename, iNewCompiledShaderObject);

        if (bOk)
            vCompiledShaderObjects.push_back(iNewCompiledShaderObject);

        if (bOk)
        {
            GLuint iNewShaderProgram;
            bOk &= ItlLinkShaderProgram(vCompiledShaderObjects, iNewShaderProgram);

            if (bOk)
            {
                // create new shader program
                TItlShader *pNewShaderProgram = new TItlShader(iNewShaderProgram, vCompiledShaderObjects);

                // check that shader program with that name is not registered yet
                assert (m_mpShaders.find(sName) == m_mpShaders.end());

                // insert into map
                m_mpShaders[sName] = pNewShaderProgram;
            }
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    GLint ShaderManager::GetAttribute(std::string sAttributeName)
    {
        GLint iLocation = -1;


        if (m_vpActiveShaderStack.size() > 0)
        {
            TItlShader *pCurrentActiveShader = m_vpActiveShaderStack.top();
            assert (pCurrentActiveShader != NULL);

            auto iterUniformLocation = pCurrentActiveShader->mCachedAttributeLocations.find(sAttributeName);

            bool bUniformLocationCached =  (iterUniformLocation != pCurrentActiveShader->mCachedAttributeLocations.end());

            if (bUniformLocationCached)
            {
                iLocation = (*iterUniformLocation).second;
            }
            else
            {
                iLocation = glGetAttribLocation(pCurrentActiveShader->nGLShaderName, sAttributeName.c_str());
               // pCurrentActiveShader->mCachedAttributeLocations[sAttributeName] = iLocation;
            }
        }

        GLenum error = glGetError();

        if (error != GL_NO_ERROR)
            Logger::error() << "glGetError(): " << GLUtils::TranslateGLerror(error) << Logger::endl;

        //assert (pCurrentActiveShader != NULL);

        return iLocation;
    }

    /***************************************************************************
     ***************************************************************************/
    GLint ShaderManager::GetUniform(std::string sUniformName)
    {
        GLint iLocation = -1;

        if (m_vpActiveShaderStack.size() > 0)
        {
            TItlShader *pCurrentActiveShader = m_vpActiveShaderStack.top();
            assert (pCurrentActiveShader != NULL);

            auto iterUniformLocation = pCurrentActiveShader->mCachedUniformLocations.find(sUniformName);

            bool bUniformLocationCached =  (iterUniformLocation != pCurrentActiveShader->mCachedUniformLocations.end());

            if (bUniformLocationCached)
            {
                iLocation = (*iterUniformLocation).second;
            }
            else
            {
                iLocation = glGetUniformLocation(pCurrentActiveShader->nGLShaderName, sUniformName.c_str());
               // pCurrentActiveShader->mCachedUniformLocations[sUniformName] = iLocation;
            }
        }

        return iLocation;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::PushActiveShader(std::string sName)
    {
        auto iterToShader = m_mpShaders.find(sName);

        bool bShaderFound = (iterToShader != m_mpShaders.end());

        if (bShaderFound)
        {
            m_vpActiveShaderStack.push((*iterToShader).second);

            glUseProgram((*iterToShader).second->nGLShaderName);
        }
        else
        {
            Logger::fatal() << "Could not activate shader with internal name \"" << sName  << "\", because it is not registered in ShaderManager" << Logger::endl;
        }

        return bShaderFound;
    }

    /***************************************************************************
     ***************************************************************************/
    void ShaderManager::PopActiveShader()
    {
        assert (m_vpActiveShaderStack.size() > 0);

        m_vpActiveShaderStack.pop();

        if (m_vpActiveShaderStack.size())
            glUseProgram(m_vpActiveShaderStack.top()->nGLShaderName);
        else
            glUseProgram(0);
    }

    bool ShaderManager::ItlLinkShaderProgram(std::vector<GLuint> vShaders, GLuint &rnResultingShaderProgram)
    {
        // create new shader program
        GLint iNewShaderProgram = glCreateProgram();

        // attach the compiled shader objects
        for (auto iter = vShaders.begin(); iter != vShaders.end(); iter++)
            glAttachShader(iNewShaderProgram, *iter);

        // link shader program
        glLinkProgram(iNewShaderProgram);

        // query link status
        GLint iLinkStatus;
        glGetProgramiv(iNewShaderProgram, GL_LINK_STATUS, &iLinkStatus);

        bool bOk = (iLinkStatus == GL_TRUE);

        if (!bOk)
        {
           ItlPrintShaderLog(iNewShaderProgram);
           Logger::fatal() << "Could not link shader program" << Logger::endl;

           glDeleteProgram(iNewShaderProgram);
        }
        else
        {
            rnResultingShaderProgram = iNewShaderProgram;

            Logger::debug() << "Sucessfully linked shader program" << Logger::endl;
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    void ShaderManager::ItlPrintShaderLog(GLuint nGLShaderOrShaderProgram)
    {
        int iLogLength = 0,
            iMaximalLength = 0;

        // get maximum length of log
        if(glIsShader(nGLShaderOrShaderProgram))
                glGetShaderiv(nGLShaderOrShaderProgram, GL_INFO_LOG_LENGTH, &iMaximalLength);
        else
                glGetProgramiv(nGLShaderOrShaderProgram, GL_INFO_LOG_LENGTH, &iMaximalLength);

        // create buffer for messages
        char *szInfoLog = new char[iMaximalLength];

        // get log messages
        if (glIsShader(nGLShaderOrShaderProgram))
                glGetShaderInfoLog(nGLShaderOrShaderProgram, iMaximalLength, &iLogLength, szInfoLog);
        else
                glGetProgramInfoLog(nGLShaderOrShaderProgram, iMaximalLength, &iLogLength, szInfoLog);

        // print log messages
        if (iLogLength > 1)
            Logger::error() << "Shader messages: \n" << szInfoLog << Logger::endl;

        // delete buffer
        delete[] szInfoLog;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::ItlLoadAndCompileShader(GLenum eShader,
                                                const std::string sFilename,
                                                GLuint &rnCreatedShaderObject)
    {
        std::vector<char> vcSource;
        bool bOk = ItlLoadSourceFromFile(sFilename, vcSource);

        if (bOk)
        {
            // create shader object
            GLuint nShaderID = glCreateShader(eShader);

            GLint iSourceLength = vcSource.size();

            const GLchar * szSource = &vcSource[0];

            // set shader source
            glShaderSource(nShaderID, 1,  (const GLchar **) &szSource, (const GLint *) &iSourceLength);

            // try to compile
            glCompileShader(nShaderID);

            // check result
            GLint iCompileStatus;
            glGetShaderiv(nShaderID, GL_COMPILE_STATUS, &iCompileStatus);

            bOk = (iCompileStatus == GL_TRUE);

            if (!bOk)
            {
                ItlPrintShaderLog(nShaderID);
                Logger::fatal() << "Could not compile shader " << sFilename << Logger::endl;
            }
            else
            {
                rnCreatedShaderObject = nShaderID;

                Logger::debug() << "Successfully compiled shader " << sFilename << Logger::endl;
            }
        }

        return bOk;
    }

    /***************************************************************************
     ***************************************************************************/
    bool ShaderManager::ItlLoadSourceFromFile(std::string sFilename, std::vector<char> &rvcSource)
    {
        // create input file stream
        std::ifstream InputFile(sFilename);

        // check if file is opened correctly
        bool bOk = InputFile.is_open();
        assert (bOk);

        // read file in one call
        if (bOk)
        {
            // seek to end
            InputFile.seekg(0, std::ios::end);

            // get position (=size)
            long lSize = InputFile.tellg();

            // seek to begin
            InputFile.seekg(std::ios::beg);

            // create buffer to read data content
            char *pBuffer = new char[lSize];

            // read content
            InputFile.read(pBuffer, lSize);

            // reserve space in dest-vector
            rvcSource.reserve(lSize);

            // clear dest vector
            rvcSource.clear();

            // copy content (todo: memcpy for faster acccess?)
            for (long i=0; i < lSize; i++)
                rvcSource.push_back(pBuffer[i]);
        }

        return bOk;
    }

}
