/*
 * header file for SceneObject_RenderState class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010
 */

#pragma once

#ifndef __realtime_sceneobject_renderstate_header
#define __realtime_sceneobject_renderstate_header

#include <glm/glm.hpp>
#include <vector>

#include "Graphics/common_gl.h"
#include "Graphics/RenderNodes/IRenderNode.h"

/**
  * A SceneObject is a object to be placed in a tree-based Scene-Structure
  *
*/

class Bamboo::RN_RenderPass : public Bamboo::IRenderNode
{
public:
    enum TRenderPass
    {
	RENDERPASS_WORLD,
	RENDERPASS_SHADOWMAP,
	//RENDERPASS_DEEP_OPACITY_MAP1,
	//RENDERPASS_DEEP_OPACITY_MAP2,

	NUM_RENDERPASSES
    };

    RN_RenderPass(TRenderPass state);
    virtual ~RN_RenderPass();

    TRenderPass getRenderState();

    void Render(std::shared_ptr<TItlRenderInfo> pRenderInfo);

protected:
    TRenderPass m_tRenderPass;

    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender() {};

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender() {};

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender() {};

	virtual void ItlPreRenderChildren() {};
	virtual void ItlPostRenderChildren() {};
    //@}

};

#endif
