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
	Shader(const char *vertexshader_filename, const char *fragmentshader_filename);

	/// Constructor which creates a shader program with a vertex shader, a geometry shader and a fragment shader
	Shader(const char *vertexshader_filename, const char *fragmentshader_filename, const char *geometryshader_filename);

	/// Constructor which creates a shader program with a vertex shader, tesselation control shader, tesselation evaluation shader and a fragment shader
	Shader(const char *vertexshader_filename, const char *tesselation_control_shader_filename, const char *tesselation_evaluation_shader_filename, const char *fragmentshader_filename);

	/// Constructor which creates a shader program with a vertex shader, tesselation control shader, tesselation evaluation shader, geometry shader and fragment shader
	Shader(const char *vertexshader_filename, const char *tesselation_control_shader_filename, const char *tesselation_evaluation_shader_filename, const char *geometry_shader_filename, const char *fragmentshader_filename);

	/// Destructor
	~Shader();
    //@}

    /*! \name Access to the shader programm */
    //@{
	/// activates the shader - attention: only one shader can be active. activating a shader deactivates the previous active shader
	void    activate();

	/// returns the position of a given uniform variable
	GLint   getUniformLocation(const char *name);

	/// returns the position of a given attribute variable
	GLint   getAttribLocation(const char *name);

	/// returns the id of the shader
	GLuint  getShaderID() { return m_nShaderId; }
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
