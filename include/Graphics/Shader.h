/*
 * header file for Shader class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010 Project "BambooIsland"
 * 03/2011 Project "Free Pool"
 */

#ifndef __realtime_lu_shader_class
#define __realtime_lu_shader_class

#include "common_gl.h"

//!  A class which encapsulates the shader handling
/*!
  This class encapsulates the handling for opengl shaders.
  It loads, compiles and link a shader (at construction time),
  and provides methods to activate it and to get the position of uniforms or attributes.
*/

#include <vector>
#include <map>
#include <string>

class Shader
{
public:
    /*! \name Constructors / Destructor */
    //@{
	/// Constructor which creates a shader program with a vertex shader and a fragment shader
        Shader(const char *szVertexShaderFilename, const char *szFragmentShaderFilename);

	/// Constructor which creates a shader program with a vertex shader, a geometry shader and a fragment shader
        Shader(const char *szVertexShaderFilename, const char *szFragmentShaderFilename, const char *szGeometryShaderFilename);

	/// Constructor which creates a shader program with a vertex shader, tesselation control shader, tesselation evaluation shader and a fragment shader
        Shader(const char *szVertexShaderFilename, const char *szTesselationControlShaderFilename, const char *szTesselationEvaluationShaderFilename, const char *szFragmentShaderFilename);

	/// Constructor which creates a shader program with a vertex shader, tesselation control shader, tesselation evaluation shader, geometry shader and fragment shader
        Shader(const char *szVertexShaderFilename, const char *szTesselationControlShaderFilename, const char *szTesselationEvaluationShaderFilename, const char *szGeometryShaderFilename, const char *szFragmentShaderFilename);

	/// Destructor
	~Shader();
    //@}

    /*! \name Access to the shader programm */
    //@{
	/// activates the shader - attention: only one shader can be active. activating a shader deactivates the previous active shader
        void    Activate();

	/// returns the position of a given uniform variable
        GLint   GetUniformLocation(const char *szName);

	/// returns the position of a given attribute variable
        GLint   GetAttribLocation(const char *szName);

	/// returns the id of the shader
        GLuint  GetShaderID() { return m_nShaderId; }
    //@}
private:
    /*! \name Internal help methods */
    //@{
	/// adds a new shader (vertex/geometry/fragment/tesselation control/tesselation evaluate) from a file to the shader program
	void ItlAddShader(GLenum tShaderType, const char *szFilename);

	/// links all added shader parts together to the shader program
	void ItlLinkShader();
    //@}

    /*! \name Static help methods */
    //@{
	/// loads a (text) file to a string (GLubyte *)
	static bool ItlLoadFileToString(const char* filename, GLubyte** ShaderSource, unsigned long* len);

	/// prints the messages of the glsl compiler
	static void ItlPrintShaderLog(GLuint obj);
    //@}

    /*! \name Private members */
    //@{
	GLuint			m_nShaderId;		    ///< the opengl-intern id of the shader (program)
    	std::vector<GLuint>	m_glShaderObjects;	    ///< the opengl-intern ids of the shader parts
    	bool			m_bReadyForUse;		    ///< wheter the shader is ready for use

	std::map<std::string, GLint>	m_mCachedUniformLocations;
	std::map<std::string, GLint>	m_mCachedAttributeLocations;
    //@}

};

#endif
