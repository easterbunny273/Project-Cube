// stl includes
#include <map>
#include <list>
#include <sstream>

// openGL, devIL
#include "common_gl.h"
#include <IL/il.h>

// assert
#include <assert.h>

// project includes
#include "TextureManager.h"
#include "PC_Logger.h"

using namespace std;

struct Bamboo::TextureManager::TImpl
{
    /*! \name Internal helper methods */
    //@{
        /// initializes DevIL
        void ItlInitialize();

        void ItlInitializeSamplerObjects();

        /// loads a texture from a file into an opengl-texture
        bool ItlLoadTextureFromFile(GLuint &rnTextureID, std::string sFilename, bool bAlreadyGammaCorrected);
    //@}

    /*! \name Private members */
    //@{
        bool m_bDevIL_Initialized;                  /// whether DevIl is initialized
        GLint m_iNumTextureUnits;                   /// num of available texture units on the GPU

        std::list<GLuint> m_lFreeUnits;             /// list of currently available (not used) texture units
        std::map<GLuint, GLuint> m_mTextureInUse;   /// maps the currently "used" textures to the corresponding texture units (to know in which unit a active texture is)

        std::map<std::string, GLuint>   m_mTextureNames;    /// maps a texture name (string) to a opengl id (GLuint)

        std::map<GLuint, GLuint> m_mLastBindedTextures;

        GLuint m_pSamplerObjects[NUM_PROVIDED_SAMPLER_OBJECTS];
    //@}
};

Bamboo::TextureManager::TextureManager()
{  
    m_pImpl = new TImpl();
    m_pImpl->m_bDevIL_Initialized = false;
}

Bamboo::TextureManager::~TextureManager()
{
    delete m_pImpl;
}

void Bamboo::TextureManager::TImpl::ItlInitialize()
{
    ilInit();

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_iNumTextureUnits);	    //ask opengl how many texture units are available

    //m_iNumTextureUnits = 32;

    for (int a=0; a < m_iNumTextureUnits; a++)			    //push them all in the free_units queue
            m_lFreeUnits.push_back(a);

    Logger::debug() << m_iNumTextureUnits << " texture units available" << Logger::endl;

    m_bDevIL_Initialized = true;

    Logger::debug() << "DevIL initialized" << Logger::endl;

    ItlInitializeSamplerObjects();
}

bool Bamboo::TextureManager::LoadTexture(std::string sTextureName,
                                         std::string sFilename,
                                         bool bAlreadyGammaCorrected)
{
    // initialize devIl if necessary
    if (m_pImpl->m_bDevIL_Initialized == false)
        m_pImpl->ItlInitialize();

    GLuint nOpenGLID;

    // load texture from file
    bool bOk = m_pImpl->ItlLoadTextureFromFile(nOpenGLID, sFilename, bAlreadyGammaCorrected);

    // if successfull, store opengl id
    if (bOk)
        m_pImpl->m_mTextureNames[sTextureName] = nOpenGLID;

    return bOk;
}


bool Bamboo::TextureManager::LoadTexture(GLuint &rnTextureID,
                                         std::string sFilename,
                                         bool bAlreadyGammaCorrected)
{
        if (m_pImpl->m_bDevIL_Initialized == false)
        {
            m_pImpl->ItlInitialize();
        }

        bool bOk = m_pImpl->ItlLoadTextureFromFile(rnTextureID, sFilename, bAlreadyGammaCorrected);

        return bOk;
}


GLuint Bamboo::TextureManager::UseTexture(std::string sTextureName)
{
    auto iter = m_pImpl->m_mTextureNames.find(sTextureName);

    assert (iter != m_pImpl->m_mTextureNames.end());

    GLuint nOpenGLID = m_pImpl->m_mTextureNames[sTextureName];

    return UseTexture(nOpenGLID);
}

GLuint Bamboo::TextureManager::UseTexture(GLuint nTextureID)
{
    // request a free unit
    GLuint nFreeUnit = RequestFreeUnit();

    // write entry to map
    m_pImpl->m_mTextureInUse[nTextureID] = nFreeUnit;

    if (m_pImpl->m_mLastBindedTextures[nFreeUnit] != nTextureID)
    {
        // activate texture unit
        glActiveTexture(GL_TEXTURE0 + nFreeUnit);

        // bind texture
        glBindTexture(GL_TEXTURE_2D, nTextureID);

        m_pImpl->m_mLastBindedTextures[nFreeUnit] = nTextureID;
    }

    return nFreeUnit;
}

void Bamboo::TextureManager::UnuseTexture(std::string sTextureName)
{
    auto iter = m_pImpl->m_mTextureNames.find(sTextureName);

    assert (iter != m_pImpl->m_mTextureNames.end());

    GLuint nOpenGLID = m_pImpl->m_mTextureNames[sTextureName];

    return UnuseTexture(nOpenGLID);
}

void Bamboo::TextureManager::UnuseTexture(GLuint nTextureID)
{
    // get map entry
    auto iter = m_pImpl->m_mTextureInUse.find(nTextureID);

    // assert that the iterator is valid (else, the texture nTextureID was not used!)
    assert (iter != m_pImpl->m_mTextureInUse.end());

    // get the used texture unit
    GLuint nUsedUnit = iter->second;

    // remove entry in map
    m_pImpl->m_mTextureInUse.erase(iter);

    // release texture unit
    ReleaseUnit(nUsedUnit);
}

GLuint Bamboo::TextureManager::RequestFreeUnit()
{
    if (m_pImpl->m_bDevIL_Initialized == false)
        m_pImpl->ItlInitialize();

    assert (m_pImpl->m_lFreeUnits.empty() == false);

    GLuint nFreeUnit = m_pImpl->m_lFreeUnits.front();	    //get first free unit

    m_pImpl->m_lFreeUnits.pop_front();			    //remove unit from free_units queue

    return nFreeUnit;
}

void Bamboo::TextureManager::ReleaseUnit(GLuint nUnit)
{
#ifdef DEBUG
  bool bOk = true;
   for (auto iter = m_pImpl->m_lFreeUnits.begin(); iter != m_pImpl->m_lFreeUnits.end() && bOk; iter++)
    {
        bOk &= (*iter != nUnit);
    }

   if (!bOk)
     Logger::fatal() << "Texture unit " << nUnit << " is released without being locked (double release?)" << Logger::endl;

#endif

    m_pImpl->m_lFreeUnits.push_back(nUnit);
}

void Bamboo::TextureManager::RegisterManualTexture(std::string sTextureName, GLuint nTextureID)
{
    Logger::debug() << "Registered manual texture \"" << sTextureName << "\" with id " << nTextureID << Logger::endl;

    unsigned int nOldSize = m_pImpl->m_mTextureNames.size();

    m_pImpl->m_mTextureNames[sTextureName] = nTextureID;

    assert (m_pImpl->m_mTextureNames.size() != nOldSize);
}

bool Bamboo::TextureManager::TImpl::ItlLoadTextureFromFile(GLuint &rnTextureID, std::string sFilename, bool bAlreadyGammaCorrected)
{
    Logger::debug() << "Try to load texture from \"" << sFilename << "\"" << Logger::endl;

    ILuint nIlTextureId;                            // index für DevIL Textur

    ilGenImages(1,&nIlTextureId);                   // generieren von IL ID für Texturen
    ilBindImage(nIlTextureId);                      // bestimmten Texturindex als aktell setzen

    if (!ilLoadImage (sFilename.data()))		// laden der Textur
    {
            ILenum Error;
            Error = ilGetError();
            ilDeleteImages(1,&nIlTextureId);		// bei Fehlschlag wieder Index freigeben

            Logger::error() << "Could not find texture file " << sFilename << ", loading failed. DevIL error-code: " << Error << Logger::endl;

            return false;
    }
    else
    {
            long int iHeight, iWidth, iFormat;
            unsigned char *szData=0;

            iWidth=ilGetInteger(IL_IMAGE_WIDTH);		    // Breite des Bildes holen
            iHeight=ilGetInteger(IL_IMAGE_HEIGHT);	    // Höhe des Bildes holen
            //bpp=ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // Farbtiefe des Bildes
            iFormat=ilGetInteger(IL_IMAGE_FORMAT);	    // Format des Bildes z.B. RGB RGBA BGR BGRA usw.
            szData=ilGetData();			    // Zeiger auf Bilddaten holen

            glGetError();				    // clear error state
            glGenTextures(1,&rnTextureID);		    // ask for opengl texture-id (returns first free id)
            glBindTexture(GL_TEXTURE_2D, rnTextureID);

            if (bAlreadyGammaCorrected)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, szData);
            else
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, szData);


            //activate mipmapping
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            float fMaximumAnisotropy=0.0;
            //get maximum ansitropic filtering value
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fMaximumAnisotropy);

            //activate 4x ansitropic filtering if possible
            if (fMaximumAnisotropy >= 1.0)
            {
                if (fMaximumAnisotropy > 4.0)
                    fMaximumAnisotropy = 4.0f;
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fMaximumAnisotropy);
                    Logger::debug() << fMaximumAnisotropy << " ansitropic filtering activated" << Logger::endl;
            }
            else
                Logger::error() << "Ansitropic filtering not available by graphic hardware" << Logger::endl;

            glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!

            GLenum eError = glGetError();
            if (eError != GL_NO_ERROR)
                Logger::error() << "glGetError: " << TranslateGLerror(eError) << Logger::endl;

            return true;
    }
}

bool Bamboo::TextureManager::IsTextureRegistered(std::string sTextureName, GLuint &rnTextureID)
{
    auto iter = m_pImpl->m_mTextureNames.find(sTextureName);

    bool bIsRegistered = (iter != m_pImpl->m_mTextureNames.end());

    if (bIsRegistered)
        rnTextureID = iter->second;

    return bIsRegistered;
}

void Bamboo::TextureManager::TImpl::ItlInitializeSamplerObjects()
{
    glGenSamplers(1 , &m_pSamplerObjects[CLAMPED_NEAREST_FILTERING]);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_NEAREST_FILTERING], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_NEAREST_FILTERING], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_NEAREST_FILTERING], GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_NEAREST_FILTERING], GL_TEXTURE_MAG_FILTER , GL_NEAREST);

    glGenSamplers(1 , &m_pSamplerObjects[CLAMPED_BILINEAR_FILTERING]);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_BILINEAR_FILTERING], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_BILINEAR_FILTERING], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_BILINEAR_FILTERING], GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_BILINEAR_FILTERING], GL_TEXTURE_MAG_FILTER , GL_LINEAR);

    glGenSamplers(1 , &m_pSamplerObjects[CLAMPED_TRILINEAR_FILTERING]);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_TRILINEAR_FILTERING], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_TRILINEAR_FILTERING], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_TRILINEAR_FILTERING], GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(m_pSamplerObjects[CLAMPED_TRILINEAR_FILTERING], GL_TEXTURE_MAG_FILTER , GL_LINEAR);


}
