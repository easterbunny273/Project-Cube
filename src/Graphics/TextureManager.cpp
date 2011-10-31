#include <IL/il.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "Graphics/common_gl.h"
#include "Graphics/TextureManager.h"
#include "Logger.h"

using namespace std;

Bamboo::TextureManager::TextureManager()
    : m_bDevIL_Initialized(false)
{
    //initialization of free_units is done with first loadTexture call because we need to make sure that the opengl context is already created
}

Bamboo::TextureManager::~TextureManager()
{

}

bool Bamboo::TextureManager::LoadTexture(std::string sTextureName,
                                         std::string sFilename,
                                         bool bAlreadyGammaCorrected,
                                         GLint iTarget /*= GL_TEXTURE_2D*/)
{
	if (m_bDevIL_Initialized == false)
	{
		ilInit();

		stringstream puffer;	//for debugging output
		string puffer2;		//for debugging output

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_iMaxTextureUnits);	    //ask opengl how many texture units are available

		puffer << m_iMaxTextureUnits;					    //for debugging output, transform integer to string, read integer in stringstream
		puffer >> puffer2;						    //for debugging output, transform integer to string, write stringstream to string


                for (int a=0; a < m_iMaxTextureUnits; a++)			    //push them all in the free_units queue
			m_lFreeUnits.push_back(a);

		Logger::debug() << puffer2 << " texture units available" << Logger::endl;

		m_bDevIL_Initialized = true;

		Logger::debug() << "DevIL initialized" << Logger::endl;
	}

	Logger::debug() << "try to load texture from \"" << sFilename << "\"" << Logger::endl;

	ILuint nIlTextureId;				// index für DevIL Textur

	ilGenImages(1,&nIlTextureId);			// generieren von IL ID für Texturen
	ilBindImage(nIlTextureId);			// bestimmten Texturindex als aktell setzen

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
		GLuint OpenGLID;

		iWidth=ilGetInteger(IL_IMAGE_WIDTH);		    // Breite des Bildes holen
		iHeight=ilGetInteger(IL_IMAGE_HEIGHT);	    // Höhe des Bildes holen
                //bpp=ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // Farbtiefe des Bildes
		iFormat=ilGetInteger(IL_IMAGE_FORMAT);	    // Format des Bildes z.B. RGB RGBA BGR BGRA usw.
		szData=ilGetData();			    // Zeiger auf Bilddaten holen

		glGetError();				    // clear error state
		glGenTextures(1,&OpenGLID);		    // ask for opengl texture-id (returns first free id)
		glBindTexture(iTarget, OpenGLID);

		if (bAlreadyGammaCorrected)
			glTexImage2D(iTarget, 0, GL_SRGB8_ALPHA8, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, szData);
		else
			glTexImage2D(iTarget, 0, GL_RGBA, iWidth, iHeight, 0, iFormat, GL_UNSIGNED_BYTE, szData);


		//activate mipmapping
		glTexParameteri(iTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(iTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		float fMaximumAnisotropy;
		//get maximum ansitropic filtering value
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fMaximumAnisotropy);

		//activate 4x ansitropic filtering if possible
		if (fMaximumAnisotropy >= 4.0)
			glTexParameterf(iTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		else
		    Logger::error() << "4x Ansitropic filtering not available by graphic hardware" << Logger::endl;

		glGenerateMipmap(iTarget);  //Generate mipmaps now!!!

		GLenum eError = glGetError();
		if (eError != GL_NO_ERROR)
		    Logger::error() << "glGetError: " << TranslateGLerror(eError) << Logger::endl;

		m_mTextureIDs[sTextureName] = OpenGLID;
		m_mTextureTargets[sTextureName] = iTarget;
		m_mTexturesInUnits[sTextureName] = -1;

		Logger::debug() << "texture from " << sFilename << " loaded to intern name \"" << sTextureName << "\"" << Logger::endl;
		return true;
	}
}


GLuint Bamboo::TextureManager::UseTexture(std::string sTextureName)
{
    unsigned int nPreviousSize = m_mTextureIDs.size();
    GLuint nTextureID = m_mTextureIDs[sTextureName];

    assert (m_mTextureLocks.find(nTextureID) == m_mTextureLocks.end() || m_mTextureLocks[nTextureID] == false);

    m_mTextureLocks[nTextureID] = true;

    //if the size of the texture-container NOW is creater than old_size,
    //the [] operator created a new entry in the map, which means that an entry with the given name did not exist
    if (nPreviousSize != m_mTextureIDs.size())
	Logger::fatal() << "Texture \"" << sTextureName << "\" could not be found (not loaded?)" << Logger::endl;
    else
    {
	if (m_lFreeUnits.size() == 0)
	    Logger::error() << "no free texture unit to use texture \"" << sTextureName << "\"" << Logger::endl;
	else
	{
	    if (m_mTexturesInUnits[sTextureName] != -1)		    //if this texture is currently stored in an unit, return the id of the unit
		return m_mTexturesInUnits[sTextureName];
	    else						    //else find a free unit and load the texture in it
	    {

#ifdef PREVENT_REBINDING_TEXTURE
		//this block loops through all units and looks if on of the units is marked as free, but has the wanted texture loaded
		//then we can reuse this unit without reloading the texture in it
		//but we must erase the unit from the free_units-list
		for (int a=0; a < m_iMaxTextureUnits; a++)
		    if (m_mUnitHasTexture[a] == nTextureID)
		    {
			for (std::list<GLuint>::iterator it1=m_lFreeUnits.begin(); it1!=m_lFreeUnits.end(); ++it1)
			    if (*it1 == a)
			    {
				m_lFreeUnits.erase(it1);
				break;
			    }
			m_mTexturesInUnits[sTextureName] = a;
			return a;
		    }
#endif

		GLuint nFreeUnit = GetFreeUnit();		    //remove unit from free_units queue
		m_mTexturesInUnits[sTextureName] = nFreeUnit;	    //make a note that this texture now is stored in a unit

#ifdef PREVENT_REBINDING_TEXTURE
		m_mUnitHasTexture[nFreeUnit] = nTextureID;	    //mark that the texture (texture_id) is stored in this unit, to prevent rebinding the texture in another unit
#endif

		assert (m_mTextureTargets.find(sTextureName) != m_mTextureTargets.end());

		// get target
		GLint iTarget = m_mTextureTargets[sTextureName];

		glActiveTexture(GL_TEXTURE0 + nFreeUnit);	    //activate given texture unit
		glBindTexture(iTarget, nTextureID);		    //load texture in this unit

		return nFreeUnit;
	    }
	}
    }

    assert (!"should not reach this point");
    return 0;
}

void Bamboo::TextureManager::UnuseTexture(std::string sTextureName)
{
    GLint iUsedUnit = m_mTexturesInUnits[sTextureName];	//which unit was used by given texture

    if (iUsedUnit != -1)
    {
	//cout << "release unit: " << used_unit << endl;
	ReleaseUnit(iUsedUnit);

	m_mTexturesInUnits[sTextureName] = -1;			//mark texture as "not stored in any unit"

	m_mTextureLocks[m_mTextureIDs[sTextureName]] = false;
    }
    else
	Logger::error() << "not possible to release texture unit used by texture \"" << sTextureName << "\", texture was not loaded in any unit!" << Logger::endl;
}

Bamboo::TextureManager *Bamboo::TextureManager::instance()
{
    static TextureManager singelton_instance;
    return &singelton_instance;
}

GLuint Bamboo::TextureManager::GetFreeUnit()
{
    GLuint nFreeUnit = m_lFreeUnits.front();	    //get first free unit

    m_lFreeUnits.pop_front();			    //remove unit from free_units queue

#ifdef PREVENT_REBINDING_TEXTURE
    m_mUnitHasTexture[nFreeUnit] = -1;		    //mark that the returned unit has no loaded texture in it
#endif

    return nFreeUnit;
}

void Bamboo::TextureManager::ReleaseUnit(GLuint nUnit)
{
    m_lFreeUnits.push_back(nUnit);
}

void Bamboo::TextureManager::RegisterManualTexture(std::string sTextureName, GLuint nTextureID, GLenum eTarget /* = GL_TEXTURE_2D */)
{
    Logger::debug() << "Registered manual texture \"" << sTextureName << "\" with id " << nTextureID << Logger::endl;

    unsigned int nOldSize = m_mTextureIDs.size();

    m_mTextureIDs[sTextureName] = nTextureID;
    m_mTextureTargets[sTextureName] = eTarget;

    m_mTexturesInUnits[sTextureName] = -1;

    assert (m_mTextureIDs.size() != nOldSize);
}

bool Bamboo::TextureManager::IsTextureRegistered(std::string sTextureName, GLuint &rnTextureID)
{
    std::map<std::string, GLuint>::iterator iter = m_mTextureIDs.find(sTextureName);
    if (iter != m_mTextureIDs.end())
    {
	rnTextureID = iter->second;
	return true;
    }
    else
	return false;
}

bool Bamboo::TextureManager::IsTextureInUse(GLuint nTextureID)
{
    std::string sTextureName;

    for (std::map<std::string, GLuint>::iterator iter = m_mTextureIDs.begin(); iter != m_mTextureIDs.end(); iter++)
	if (iter->second == nTextureID)
	{
	    sTextureName = iter->first;
	    break;
	}

    assert (sTextureName.empty() == false);
    assert (m_mTexturesInUnits.find(sTextureName) != m_mTexturesInUnits.end());

    return (m_mTexturesInUnits[sTextureName] != -1);
}

void Bamboo::TextureManager::LockTextureID(GLuint nTextureID)
{
    m_mTextureLocks[nTextureID] = true;
}

void Bamboo::TextureManager::UnlockTextureID(GLuint nTextureID)
{
    m_mTextureLocks[nTextureID] = false;
}

GLuint Bamboo::TextureManager::CreateSampler(std::string sTextureName, GLenum eTarget, GLint iInternalTextureFormat, GLint iWidth, GLint iHeight, GLint iClampMode, GLint iFiltering)
{
    GLuint nTextureID;

    GLuint nAssignedUnit = GetFreeUnit();

    // clear errors
    glGetError();

    // switch to assigned texture unit
    glActiveTexture(GL_TEXTURE0 + nAssignedUnit);

    // generate texture (=create new opengl id)
    glGenTextures(1, &nTextureID);

    // bind color texture
    glBindTexture(eTarget, nTextureID);

    // set texture format and data
    glTexImage2D(eTarget, 0, iInternalTextureFormat, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // set texture parameters
    glTexParameterf(eTarget, GL_TEXTURE_WRAP_S, iClampMode);
    glTexParameterf(eTarget, GL_TEXTURE_WRAP_T, iClampMode);
    glTexParameterf(eTarget, GL_TEXTURE_MAG_FILTER, iFiltering);
    glTexParameterf(eTarget, GL_TEXTURE_MIN_FILTER, iFiltering);

    // unbind texture
    glBindTexture(eTarget, 0);

    GLenum eError = glGetError();

    assert (eError == GL_NO_ERROR);

    if (eError != GL_NO_ERROR)
	Logger::error() << __FUNCTION__ << ": Could not create a sampler!" << Logger::endl;

    // release unit
    ReleaseUnit(nAssignedUnit);

    // register texture
    RegisterManualTexture(sTextureName, nTextureID, eTarget);

    // return open gl texture id
    return nTextureID;
}

GLint Bamboo::TextureManager::GetTextureTarget(std::string sTextureName)
{
    unsigned int nOldSize = m_mTextureTargets.size();

    GLint iTarget = m_mTextureTargets[sTextureName];

    // check that size has not changed (else the entry did not exist before)
    assert (m_mTextureTargets.size() == nOldSize);

    return iTarget;
}

bool Bamboo::TextureManager::IsTextureLocked(GLuint texture_id)
{
    return !(m_mTextureLocks.find(texture_id) == m_mTextureLocks.end() || m_mTextureLocks[texture_id] == false);
}
