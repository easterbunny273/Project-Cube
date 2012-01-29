/*
 * header file for TextureManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, 06/2011
 */

#ifndef __realtime_lu_texture_manager_header
#define __realtime_lu_texture_manager_header

#include <string>

#include "common_gl.h"
#include "Graphic.h"

/*! \brief A class for loading textures and manage the use of the texture units.
 *
 *  This class provides methods for loading textures from image files,
 *  and for binding/unbinding textures to a free texture unit,
 *  where the caller must not think about the use of the texture units.
 */

class Bamboo::TextureManager
{
public:

    /*! \name Public types */
    //@{
    class Texture
    {
    public:

    private:
      /*! \name Construction / Destruction */
      //@{
      // private constructor
      Texture();

      //@}

      /*! \name Private members */
      //@{
          GLuint m_nTextureID;
          GLuint m_nSamplerID;
      //@}
    };

    enum TProvidedDefaultSamplerObjects
    {
        CLAMPED_NEAREST_FILTERING = 0,
        CLAMPED_BILINEAR_FILTERING,
        CLAMPED_TRILINEAR_FILTERING,
        CLAMPED_ANSITROPIC_FILTERING,

        NUM_PROVIDED_SAMPLER_OBJECTS
    };

    //@}

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

    /// binds a texture to a free texture unit and returns the used unit
    GLuint UseTexture(std::string sTextureName);

    /// binds a texture to a free texture unit and returns the used unit
    GLuint UseTexture(GLuint nOpenGLID);

    /// marks the texture unit used by this texture as "free"
    void UnuseTexture(std::string sTextureName);

    /// marks the texture unit used by this texture as "free"
    void UnuseTexture(GLuint nOpenGLID);

    /// returns a free texture unit and marks it as "used"
    GLuint RequestFreeUnit();

    /// releases the given texture unit
    void ReleaseUnit(GLuint nUnit);

    /// registers a existing opengl-texture, makes it accessable with the given name
    void RegisterManualTexture(std::string sTextureName, GLuint nTextureID);

    /// returns whether the given texture (sTextureName) is registered, and returns the opengl-id if so
    bool IsTextureRegistered(std::string sTextureName, GLuint &rnTextureID);
    //@}

private:
    /*! \name Private members */
    //@{
    struct TImpl;
    //@}

    /*! \name Private members */
    //@{
    TImpl * m_pImpl;        /// pimpl pattern :-)
    //@}
};

#endif
