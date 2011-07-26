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

class TextureManager
{
private:
    /*! This variable marks if devIL is initialized yet (because the opengl context must be initialized before) */
    bool m_bDevIL_Initialized;

    /*! How many texture units are available on the gpu? */
    GLint m_iMaxTextureUnits;

    /*! This map connects string TextureNames (set by user) with opengl-internal texture names (=texture id)*/
    std::map<std::string, GLuint> m_mTextureIDs;

    /*! This map connects string TextureNames (set by user) with opengl-internal targets*/
    std::map<std::string, GLint> m_mTextureTargets;

    /*! This map holds all textures (first parameter) which are currently stored in texture units (second parameter) */
    std::map<std::string, GLint> textures_in_units;
    std::list<GLuint> free_units;

    std::map<GLuint, bool> m_mTextureLocks;

#ifdef PREVENT_REBINDING_TEXTURE
    /*! this map holds the texture_ids which where loaded in the units (first parameter: unit, second parameter: texture id) */
    std::map<GLint, GLuint> unit_has_texture;
#endif

    /*! \brief Constructor, only for internal use (singelton pattern) */
    TextureManager();

public:
    /*! \brief Returns the singelton instance */
    static TextureManager *instance();

    /*! \brief Destructor */
    ~TextureManager();

    /*! \brief Method for loading a texture from file
     *
     *  This method loads a texture from a file, creates a new opengl-texture and makes it accessable over the given name
     *  \param sTextureName The wanted name to make the texture accessable, must be unique
     *  \param sFilename The filename (relative to workdir) of the file which should be load
     *  \param bAlreadyGammaCorrected Specify if the image which should be loaded is already gamma-corrected or not
     *  \sa TextureManager::useTexture(), TextureManager::unuseTexture()
     */
    bool loadTexture(std::string sTextureName, std::string sFilename, bool bAlreadyGammaCorrected, GLint iTarget = GL_TEXTURE_2D);

    /*! \brief Method to use an already loaded texture
     *
     *  This method binds an already loaded texture to a free texture
     *  unit, marks the unit as used and returns the ID of the unit.
     *  In case that there is no free texture unit, the method sends a fatal-message
     *  to the logger which forces the program to quit.
     *
     *  To ensure that there are always enough free texture units, you must call
     *  TextureManager::unuseTexture(...) as soon as possible for you.
     *
     *  \param sTextureName The name of the texture
     *  \return The ID of the used texture unit
     *  \sa TextureManager::loadTexture(), TextureManager::unuseTexture()
     */
    GLuint useTexture(std::string sTextureName);

    /*! \brief Method to unuse a texture
     *
     *  This method marks the texture unit which is currently used
     *  for this texture as free. The texture unit is not cleared,
     *  but each call of useTexture(...) may allocate the unit again and
     *  bind another texture to this unit. Therefore, to use a texture again,
     *  you have to call TextureManager::useTexture() again.
     *
     *  In case that the given texture is not bound to any unit and you call unuseTexture(...),
     *  the method will print an error message.
     *
     *  \param sTextureName The name of the texture
     *  \sa TextureManager::loadTexture(), TextureManager::useTexture()
     */
    void unuseTexture(std::string sTextureName);

    /*! \brief Returns the ID of a free unit and marks it as used
     *
     *  This method returns the ID of the first free unit
     *  and marks it as used.
     *
     *  Normally, this method is used internally by useTexture()
     *
     *  If you use this method on your own, you must release the unit as soon as possible with
     *  TextureManager::releaseUnit().
     *
     *  \return The ID of a free texture unit.
     *  \sa TextureManager::releaseUnit(), TextureManager::useTexture()
     */
    GLuint getFreeUnit();

    /*! \brief Marks a given texture unit as free
     *
     *  This method marks the given texture unit as free
     *
     *  Normally, this method is used internally by unuseTexture()
     *
     *  \param nUnit The id of the texture unit
     *  \sa TextureManager::getFreeUnit(), TextureManager::unuseTexture()
     */
    void releaseUnit(GLuint nUnit);

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
    void registerManualTexture(std::string sTextureName, GLuint nTextureID, GLenum eTarget = GL_TEXTURE_2D);

    /// returns wheter this texture is registered in the texture manager, and returns the opengl id if it is registered
    bool isTextureRegistered(std::string sTextureName, GLuint &rnTextureID);

    /// returns if this texture is currently in use
    bool isTextureInUse(GLuint nTextureID);

    /// returns if this texture is currently locked (in use)
    bool isTextureLocked(GLuint nTextureID);


    /// locks a texture (marks it as in use)
    void lockTextureID(GLuint nTextureID);

    /// unlocks a texture
    void unlockTextureID(GLuint nTextureID);

    /// creates a sampler according to the given parameters and registers it in the texture manager,
    /// and returns the opengl id of the created texture
    GLuint CreateSampler(std::string sTextureName, GLenum eTarget, GLint iInternalTextureFormat, GLint iWidth, GLint iHeight, GLint iClampMode, GLint iFiltering);

    /// returns the target of the texture
    GLint GetTextureTarget(std::string sTextureName);
};

#endif
