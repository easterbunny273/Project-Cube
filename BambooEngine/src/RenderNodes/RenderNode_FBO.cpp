#include <string.h>

#include "RenderNodes/RenderNode_FBO.h"
#include "TextureManager.h"
#include "PC_Logger.h"


/*! \brief Constructor for creating a fbo with a color texture attached
 *
 *  This constructor creates a SceneObject which encapsulates a fbo with an attached color texture.
 *  All child objects of this SceneObject get rendered in this fbo and the resulting color texture
 *  can be accessed through the given name (szColorTextureName) of the texture (by using the TextureManager)
 *  \param iWidth The wanted with of the fbo
 *  \param iHeight THe wanted height of the fbo
 *  \param szColorTextureName The texture name which is used to register the color texture at the TextureManager
 *  \param bFloating16 If true, the generated color texture used 16bit floating precision.
 *  \param bMipMapped If true, mipmaps for the color texture are generated after each update of the color texture
 */
Bamboo::RN_FBO::RN_FBO(int iWidth,
                                 int iHeight,
                                 const char *szColorTextureName,
                                 bool bFloating16, bool bMipMapped)
    : m_bMipMapped(bMipMapped),
      m_iWidth(iWidth),
      m_iHeight(iHeight),
      m_szColorTextureName(szColorTextureName),
      m_szDepthTextureName(0)
{
    // get texture manager
    Bamboo::TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    //write down which rendertargets we use
    m_bColorTexture = true;	    //we use a color texture
    m_bDepthTexture = false;	    //we use no depth texture
    m_bDepthRenderbuffer = true;    //but we use a renderbuffer for the depth

    //get the id of a free texture unit from the texture manager
    GLuint textureUnit = pTextureManager->RequestFreeUnit();

    //activate this unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);


    //bool bNewTexture = false;

    if (pTextureManager->IsTextureRegistered(szColorTextureName, m_nColorTexture) == false)
    {

	//generate color texture (=create new opengl id)
	glGenTextures(1, &m_nColorTexture);

	//bind color texture
	glBindTexture(GL_TEXTURE_2D, m_nColorTexture);

	//set texture format and data
	if (bFloating16 == false)
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	else
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	//set texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (bMipMapped)
	{
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//register color texture in texture manager, for easy use by simple calling TextureManager::useTexture(szColorTextureName);
        pTextureManager->RegisterManualTexture(szColorTextureName, m_nColorTexture);

//	bNewTexture = true;
    }
    else
	Logger::debug() << "new fbo, already registered texture used (" << m_nColorTexture << ", " << szColorTextureName << Logger::endl;

    //release unit
    pTextureManager->ReleaseUnit(textureUnit);

    //generate renderbuffer
    glGenRenderbuffers(1, &m_nDepthRenderbuffer);

    //bind renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthRenderbuffer);

    //set renderbuffer format and data
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight);

    //unbind renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    //generate frame buffer
    glGenFramebuffers(1, &m_nFramebuffer);

    //bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);

    //attach color texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nColorTexture, 0);

    //attach depth renderbuffer to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthRenderbuffer);

    //check framebuffer status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    //check if its complete
    if(status != GL_FRAMEBUFFER_COMPLETE)
	Logger::fatal() << "Failed to initialize FBO" << Logger::endl;
    else
	Logger::debug() << "Initialized FBO" << Logger::endl;

    //unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*! \brief Constructor for creating a fbo with a color- and depth texture attached
 *
 *  This constructor creates a SceneObject which encapsulates a fbo with an attached color texture
 *  and an attached depth texture.
 *  All child objects of this SceneObject get rendered in this fbo and the resulting color and depth textures
 *  can be accessed through the given names (by using the TextureManager)
 *  \param iWidth The wanted with of the fbo
 *  \param iHeight THe wanted height of the fbo
 *  \param szColorTextureName The texture name which is used to register the color texture at the TextureManager
 *  \param szDepthTextureName The texture name which is used to register the depth texture at the TextureManager
 *  \param bMipMapped If true, mipmaps for the color texture are generated after each update of the color texture
 */
Bamboo::RN_FBO::RN_FBO(int iWidth,
                                 int iHeight,
                                 const char *szColorTextureName,
                                 const char *szDepthTextureName,
                                 bool bMipMapped)
    : m_bMipMapped(bMipMapped),
      m_iWidth(iWidth),
      m_iHeight(iHeight),
      m_szColorTextureName(szColorTextureName),
      m_szDepthTextureName(szDepthTextureName)
{
    // get texture manager
    Bamboo::TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    //write down which rendertargets we use
    m_bColorTexture = true;	    //we use a color texture
    m_bDepthTexture = true;	    //we use a depth texture
    m_bDepthRenderbuffer = false;   //so we don't use a depth renderbuffer

    //get the id of a free texture unit from the texture manager
    GLuint textureUnit = pTextureManager->RequestFreeUnit(); //ask for a free texture unit

    //activate unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    //generate color texture (=create new opengl id)
    glGenTextures(1, &m_nColorTexture);

    //bind color texture
    glBindTexture(GL_TEXTURE_2D, m_nColorTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


    //set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //if mipmapping is activated, set filter to mipmap and generate mipmaps
    if (bMipMapped)
    {
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //generate depth texture
    glGenTextures(1, &m_nDepthTexture);

    //bind depth texture
    glBindTexture(GL_TEXTURE_2D, m_nDepthTexture);

    //set texture format and data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);


    //set texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //register color texture in texture manager, for easy use by simple calling TextureManager::useTexture(szColorTextureName);
    pTextureManager->RegisterManualTexture(szColorTextureName, m_nColorTexture);

    //register depth texture in texture manager, for easy use by simple calling TextureManager::useTexture(szColorTextureName);
    pTextureManager->RegisterManualTexture(szDepthTextureName, m_nDepthTexture);

    glGenFramebuffers(1, &m_nFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);

    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nColorTexture, 0);

    // attach the renderbuffer to depth attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthTexture, 0);

    //check fbo status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
	Logger::fatal() << "Failed to initialize FBO" << Logger::endl;
    else
	Logger::debug() << "Initialized FBO" << Logger::endl;

    //release used texture unit
    pTextureManager->ReleaseUnit(textureUnit);

    //unbind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Bamboo::RN_FBO::~RN_FBO()
{
#pragma warn "todo: correct this"
    //delete color texture, if used
    /*if (m_bColorTexture)
	glDeleteTextures(1, &m_nColorTexture);*/

    //delete depth texture, if used
    if (m_bDepthTexture)
	glDeleteTextures(1, &m_nDepthTexture);

    //delete depth renderbuffer, if used
    if (m_bDepthRenderbuffer != -1)
	glDeleteRenderbuffers(1, &m_nDepthRenderbuffer);

    //finally, delete the used fbo
    glDeleteFramebuffers(1, &m_nFramebuffer);
}

/*!
 *  This method is called internally BEFORE rendering the children,
 *  and binds and clears the fbo as needed.
 */
void Bamboo::RN_FBO::ItlPreRenderChildren()
{
    //bind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_nFramebuffer);

    //set viewport size
    glViewport(0,0, m_iWidth, m_iHeight);

    // clear buffers
   // glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///store our fbo-id on the stack, to allow "deeper" SceneObject_FBOs to rebind our fbo after finish drawing
    ItlPushFBO(m_nFramebuffer);

    ItlPushViewportInformation(m_iWidth, m_iHeight);
}

/*!
 *  This method is called internally AFTER rendering the children,
 *  and unbinds the fbo.
 */
void Bamboo::RN_FBO::ItlPostRenderChildren()
{

    //if mipmapping is activated, update mipmaps
    if (m_bMipMapped)
    {
        // get texture manager
        Bamboo::TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
        assert (pTextureManager != NULL);

        pTextureManager->UseTexture(m_szColorTextureName);
	glGenerateMipmap(GL_TEXTURE_2D);
        pTextureManager->UnuseTexture(m_szColorTextureName);
    }

    //remove the fbo (THIS fbo) from the bound_fbos stack
    ItlPopFBO();

    ItlPopViewportInformation();

    //if there was a SceneObject_FBO bound in the SceneObject_Tree, rebind the previous bound fbo
    if (ItlIsNestedFBO())
    {
        GLuint previous_bound = ItlGetTopFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, previous_bound);
    }
    else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int iOldHeight, iOldWidth;

    ItlGetTopViewportInformation(iOldWidth, iOldHeight);

    //restore viewport params
    glViewport(0, 0, iOldWidth, iOldHeight);
}

void Bamboo::RN_FBO::ItlPreRender()
{

}

void Bamboo::RN_FBO::ItlRender()
{

}

void Bamboo::RN_FBO::ItlPostRender()
{

}
