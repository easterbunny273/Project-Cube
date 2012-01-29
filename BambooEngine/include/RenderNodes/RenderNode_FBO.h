/*
 * header file for SceneObject_FBO class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 */

#ifndef __realtime_sceneobject_fbo_header
#define __realtime_sceneobject_fbo_header

#include "RenderNodes/IRenderNode.h"

//!  A SceneObject which renders all children in a fbo.
/*!
  This SceneObject is not a real SceneObject as an object in the scene,
  rather it's an object which can be used in the sceneobjects-tree to render all
  children in a fbo.
  This object renders all children in a fbo and registers the created textures in the TextureManager,
  which means that the resulting textures can simply used by TextureManager::useTexture(...).
  The name for the textures must be set in the used constructor.
*/

class Bamboo::RN_FBO : public Bamboo::IRenderNode
{
private:
    bool m_bColorTexture;		///<shows if this fbo uses a color texture
    bool m_bDepthTexture;		///<shows if this fbo uses a depth texture
    bool m_bDepthRenderbuffer;		///<shows if this fbo uses a depth renderbuffer
    bool m_bMipMapped;			///<shows if this fbo uses mipmaps, which means that the colortexture is mipmapped after each render update

    GLuint m_nFramebuffer;		//internal opengl internal id for fbo
    GLuint m_nColorTexture;		//internal opengl id for color texture
    GLuint m_nDepthTexture;		//internal opengl id for depth texture

    GLuint m_nDepthRenderbuffer;	//renderbuffer for depth buffer

    int m_iWidth;			//width of the fbo
    int m_iHeight;			//height of the fbo

    const char *m_szColorTextureName;		//name of the color texture, if the fbo uses one, uses for registering in texture manager
    const char *m_szDepthTextureName;		//name of the depth texture, if the fbo uses one, uses for registering in texture manager

protected:
    /*! \name SceneObject Interface */
    //@{
	/*! this method is called before the render() method calls the render() methods of the children,
	    and can be used to bind a fbo (to render the children in this fbo) or something like that */
	virtual void ItlPreRenderChildren();

	/*! this method is called after all children were rendered,
	    and can be used to unbind a fbo or something like that */
	virtual void ItlPostRenderChildren();

	/*! this method is called before the sceneobject itself gets rendered.
	    shaders and things like that should be activated in this method */
	virtual void ItlPreRender();

	/*! this method is called to render the sceneobject.
	    Attention: If the correct shader program is not bound yet (should be done in itlBeforeRender()),
	    the transform matrices must be sent again */
	virtual void ItlRender();

	/*! this method is called after rendering the sceneobject itself. Cleaning up can be done here */
	virtual void ItlPostRender();
    //@}
public:

//    SceneObject_FBO(int iWidth, int iHeight, const char *szColorTextureName, int iDummy);

    ///Constructor for a SceneObject_FBO, creates a color texture and a renderbuffer for the depth.
    RN_FBO(int iWidth, int iHeight, const char *szColorTextureName, bool bFloating16 = false, bool bMipMapped = false);

    ///Constructor for a SceneObject_FBO, creates a color texture and a depth texture.
    RN_FBO(int iWidth, int iHeight, const char *szColorTextureName, const char *szDepthTextureName, bool bMipMapped = false);

    ///Destructor of the fbo, releases ressources
    ~RN_FBO();
};

#endif
