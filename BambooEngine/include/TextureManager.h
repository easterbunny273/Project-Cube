/*
 * header file for TextureManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, 06/2011
 */

#ifndef __realtime_lu_texture_manager_header
#define __realtime_lu_texture_manager_header

#include <string>
#include <map>
#include <list>
#include "common_gl.h"
#include "Graphic.h"

/* if PREVENT_REBINDING_TEXTURE is defined, the texturemanager tries to prevent rebinding
 * a texture if a texture is still bound to an unit but the unit is marked as "free for used".
 * In this case, the texturemanager just marks the unit as "used" and prevents rebinding - but
 * I think that the overhead to do this beats the benefit, so its disabled by default */
//#define PREVENT_REBINDING_TEXTURE

/*! \brief A class for loading textures and manage the use of the texture units.
 *
 *  This class provides methods for loading textures from image files,
 *  and for binding/unbinding textures to a free texture unit,
 *  where the caller must not think about the use of the texture units.
 */

class Bamboo::TextureManager
{


public:
    /*! \name Construction / Destruction */
    //@{
    /// constructor
    TextureManager();

    /// destructor
    ~TextureManager();
    //@}

    /*! \name Public methods */
    //@{
    /// loads a texture from a file and makes it accessable with the given sTextureName,
    /// returns true if successfull and false if not
    bool LoadTexture(std::string sTextureName,
                     std::string sFilename,
                     bool bAlreadyGammaCorrected);

    /// loads a texture from a file and writes the opengl-id in the given rnTextureID
    /// returns true if successfull and false if not
    bool LoadTexture(GLuint &rnTextureID,
                     std::string sFilename,
                     bool bAlreadyGammaCorrected);

    //@}

    void Initialize();

    /*! \brief Returns the singelton instance */
    static TextureManager *instance();


    GLuint UseTexture(std::string sTextureName);
    GLuint UseTexture(GLuint nOpenGLID);

    void UnuseTexture(std::string sTextureName);
    void UnuseTexture(GLuint nOpenGLID);

    GLuint RequestFreeUnit();
    void ReleaseUnit(GLuint nUnit);

    /*! \brief Registers an initialized opengl-texture in TextureManager to use it with TextureManager::useTexture()
     *
     *  This method registers an initialized (and ready for use) opengl-texture
     *  in the internal structures of the TextureManager-class,
     *  to make it available through TextureManager::useTexture(...) calls
     *
     *  This method can be used if you create your textures on your own instead of using TextureManager::loadTexture()
     *  \param sTextureName The wanted name to make the texture accessable, must be unique
     *  \param nTextureID The opengl name of the texture (=the texture ID)
     *
     *  \sa TextureManager::useTexture(), TextureManager::loadTexture()
     */
    void RegisterManualTexture(std::string sTextureName, GLuint nTextureID, GLenum eTarget = GL_TEXTURE_2D);

    /// returns wheter this texture is registered in the texture manager, and returns the opengl id if it is registered
    bool IsTextureRegistered(std::string sTextureName, GLuint &rnTextureID);

    /// returns if this texture is currently in use
    bool IsTextureInUse(GLuint nTextureID);

    /// returns if this texture is currently locked (in use)
    bool IsTextureLocked(GLuint nTextureID);


    /// locks a texture (marks it as in use)
    void LockTextureID(GLuint nTextureID);

    /// unlocks a texture
    void UnlockTextureID(GLuint nTextureID);

    /// creates a sampler according to the given parameters, registers it in the texture manager,
    /// and returns the opengl id of the created texture
    GLuint CreateSampler(std::string sTextureName, GLenum eTarget, GLint iInternalTextureFormat, GLint iWidth, GLint iHeight, GLint iClampMode, GLint iFiltering);

    /// returns the target of the texture
    GLint GetTextureTarget(std::string sTextureName);


private:
    /*! \name Internal helper methods */
    //@{
    //@}

    /*! This variable marks if devIL is initialized yet (because the opengl context must be initialized before) */
    bool m_bDevIL_Initialized;

    /*! How many texture units are available on the gpu? */
    GLint m_iMaxTextureUnits;

    /*! This map connects string TextureNames (set by user) with opengl-internal texture names (=texture id)*/
    std::map<std::string, GLuint> m_mTextureIDs;

    /*! This map connects string TextureNames (set by user) with opengl-internal targets*/
    std::map<std::string, GLint> m_mTextureTargets;

    /*! This map holds all textures (first parameter) which are currently stored in texture units (second parameter) */
    std::map<std::string, GLint> m_mTexturesInUnits;
    std::list<GLuint> m_lFreeUnits;

    std::map<GLuint, bool> m_mTextureLocks;

#ifdef PREVENT_REBINDING_TEXTURE
    /*! this map holds the texture_ids which where loaded in the units (first parameter: unit, second parameter: texture id) */
    std::map<GLint, GLuint> m_mUnitHasTexture;
#endif

    bool ItlLoadTextureFromFile(GLuint &rnTextureID, std::string sFilename, bool bAlreadyGammaCorrected);

    /*! \name Private members */
    //@{
    //@}
};

#endif
