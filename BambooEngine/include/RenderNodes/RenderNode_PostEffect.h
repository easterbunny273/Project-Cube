/*
 * header file for SceneObject_PostEffect class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2010, Projekt "Bamboo Island"
 * 03/2011, Projekt "Free Pool"
 * 04/2011, Projekt "Bamboo Engine"
 */

#pragma once

#ifndef __realtime_sceneobject_posteffect_header
#define __realtime_sceneobject_posteffect_header

#include <string>
#include <map>
#include <glm/glm.hpp>
#include "RenderNodes/IRenderNode.h"

//!  A SceneObject which draws a texture full screen.
/*!
  This SceneObject is not a real SceneObject as an object in the scene,
  rather it's an object which draws a given texture without projection over the whole viewport.
  This is useful for applying post screen effects (rendering scene in a fbo, and drawing resulting color texture fullscreened with a given shader).
*/

class Bamboo::RN_PostEffect : public Bamboo::IRenderNode
{
public:
    /*! \name Constructors / Destructor */
    //@{
	/// This constructor takes the name of shader to use, and the name of one or two textures to pass it on the shader
        RN_PostEffect(std::string sShaderToUse);
        ~RN_PostEffect();
    //@}

    /*! \name Public methods */
    //@{
	void SetUniform(std::string sUniform, float fValue);
	void SetUniform(std::string sUniform, glm::vec2 v2Value);
	void SetUniform(std::string sUniform, glm::vec3 v3Value);

        void SetTexture(std::string sUniformName, std::string sTextureName);
        void SetTexture(std::string sUniformName, GLuint nTextureID);
    //@}

protected:
     /*! \name SceneObject Interface */
     //@{
	 /*! this method is called before the sceneobject itself gets rendered.
	     shaders and things like that should be activated in this method */
	 virtual void ItlPreRender();

	 /*! this method is called to render the sceneobject.
	     Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	     the transform matrices must be sent again */
	 virtual void ItlRender();

	 /*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	 virtual void ItlPostRender();

	virtual void ItlPreRenderChildren() {};
	virtual void ItlPostRenderChildren() {};
     //@}

private:
    /*! \name SceneObject Interface */
    //@{
         std::map<std::string, float>       m_mUniforms_Floats;
         std::map<std::string, glm::vec2>    m_mUniforms_Vec2;
         std::map<std::string, glm::vec3>    m_mUniforms_Vec3;
         std::map<std::string, std::string> m_mTextures;
         std::map<std::string, GLuint>      m_mTexturesDirect;

         GLuint buffer_vertices3f;
         GLuint vao;

         std::string m_sShaderName;
         std::string m_sTextureName;
         std::string m_sTextureName2;
         std::string m_sTextureName3;
     //@}

};

#endif
