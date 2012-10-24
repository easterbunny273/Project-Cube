/*
 * header file for ShaderManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 * 10/2012, Project "BambooGraphics"
 */

#pragma once

#ifndef __BAMBOOGRAPHICS_SHADERMANAGER_H
#define __BAMBOOGRAPHICS_SHADERMANAGER_H

#include <vector>
#include <string>
#include <stack>
#include "common_gl.h"
#include "Graphic.h"

namespace BambooGraphics
{
    //!  A class which handles many Shader classes
    /*!
      This class supports handling of many shader classes. It provides
      methods for nested activating of shaders (push the current Shader, activate another and pop (reactivate) the previous one).

      Furthermore, it provides methods to lock the active shader (all activateShader(...) calls are ignored),
      which can be used for things like enforcing a simple shader program when rendering the shadowmap or reflected objects.
    */

    class ShaderManager
    {
    public:
        /*! \name Constructor / Destructor */
        //@{
        /// Constructor, private because the class is designed as a Singelton
        ShaderManager();

        /// Destructor, destroys all used shaders programs
        ~ShaderManager();
        //@}

        /*! \name Public methods */
        //@{
            /// Creates a shader program containing a vertex shader and a fragment shader, returns true if successful.
            bool CreateAndRegisterShader(const std::string sName,
                                         const std::string sVertexShaderFilename,
                                         const std::string sFragmentShaderFilename);

            /// Creates a shader program containing a vertex shader, a geometry shader
            /// and a fragment shader, returns true if successful.
            bool CreateAndRegisterShader(const std::string sName,
                                         const std::string sVertexShaderFilename,
                                         const std::string sGeometryShaderFilename,
                                         const std::string sFragmentShaderFilename);

            /// Creates a shader program containing a vertex shader, tesselation shaders
            /// and a fragment shader, returns true if successful
            bool CreateAndRegisterShader(const std::string sName,
                                         const std::string sVertexShaderFilename,
                                         const std::string sTesselationControlShaderFilename,
                                         const std::string sTesselationEvaluationShaderFilename,
                                         const std::string sFragmentShaderFilename);

            /// Creates a shader program containing a vertex shader, tesselation shaders,
            /// a geometry shader and a fragment shader, returns true if successful
            bool CreateAndRegisterShader(const std::string sName,
                                         const std::string sVertexShaderFilename,
                                         const std::string sTesselationControlShaderFilename,
                                         const std::string sTesselationEvaluationShaderFilename,
                                         const std::string sGeometryShaderFilename,
                                         const std::string sFragmentShaderFilename);

            /// Activates the given shader and pushes it on the stack
            bool PushActiveShader(std::string sName);

            /// Pops the top shader from the stack and activates the new top
            void PopActiveShader();
        //@}

        /*! \name Public attributes */
        //@{
        /// returns the location of an uniform variable within the current active shader program
            GLint GetUniform(std::string sUniformName);

        /// returns the location of an attribute variable within the current active shader program
            GLint GetAttribute(std::string sAttributeName);
        //@}

    private:
        /*! \name Internal types */
        //@{
            struct TItlShader
            {
                GLuint                          nGLShaderName;
                std::vector<GLuint>             vCompiledShaderObjects;
                std::map<std::string, GLint>    mCachedUniformLocations;
                std::map<std::string, GLint>    mCachedAttributeLocations;

                TItlShader(GLuint _nGLProgram, std::vector<GLuint> _vGLShaders) : nGLShaderName(_nGLProgram), vCompiledShaderObjects(_vGLShaders) {}
            };

        //@}

        /*! \name Internal helper methods */
        //@{
            /// adds a new shader (vertex/geometry/fragment/tesselation control/tesselation evaluate) from a file to the shader program
            void ItlAddShader(GLuint nShaderProgram, GLenum tShaderType, const char *szFilename);

            bool ItlLoadSourceFromFile(std::string sFilename, std::vector<char> &rvcSource);

            bool ItlLoadAndCompileShader(GLenum eShader, const std::string sFilename, GLuint &rnCreatedShaderObject);

            bool ItlLinkShaderProgram(std::vector<GLuint> vShaders, GLuint &rnResultingShaderProgram);

            /// prints the messages of the glsl compiler
            void ItlPrintShaderLog(GLuint nGLShaderOrShaderProgram);
        //@}

        /*! \name Private members */
        //@{
        std::map<std::string, TItlShader *> m_mpShaders;

        std::stack<TItlShader *>	m_vpActiveShaderStack;		///< the stack for nested activating, push/pop
        //@}
    };
};

#endif //__BAMBOOGRAPHICS_SHADERMANAGER_H
