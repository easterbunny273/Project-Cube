/*
 * header file for ShaderManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 */

#pragma once

#ifndef __realtime_lu_shadermanager_class
#define __realtime_lu_shadermanager_class

#include <vector>
#include <string>
#include <stack>
#include "common_gl.h"
#include "Shader.h"

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
    /*! \name Singelton getter */
    //@{
	static ShaderManager *instance();
    //@}

    /*! \name Public methods */
    //@{
	/// adds a shader program to the manager, from now on the shader can be activated with activateShader(name),
	/// and the Shader instance is destroyed when the ShaderManager gets destroyed.  \sa activateShader()
        void AddShader(std::string sName, Shader *pShader);

	/// activates the given shader if possible, else the method prints an error message to the Logger. \sa Logger
        bool ActivateShader(std::string sName);

	/// pushs the active shader on the stack
        void PushActiveShader();

	/// pops the top shader from the stack and activates it
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
    /*! \name Constructor / Destructor */
    //@{
	/// Constructor, private because the class is designed as a Singelton
	ShaderManager();

	/// Destructor, destroys all used shaders programs
	~ShaderManager();
    //@}

    /*! \name Private members */
    //@{
	unsigned int			m_nCurrentActiveShaderProgram;	    ///< The id of the currently active shader

	std::vector<Shader *>		m_vpShaders;			///< the shader instances
	std::vector<std::string>	m_vsShaderNames;		///< the name for the shader instances
	std::stack<unsigned int>	m_vActiveShaderStack;		///< the stack for nested activating, push/pop
    //@}
};

#endif
