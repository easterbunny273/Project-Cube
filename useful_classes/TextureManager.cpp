#include <IL/il.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "common_gl.h"
#include "TextureManager.h"
#include "Logger.h"

using namespace std;

TextureManager::TextureManager() : m_bDevIL_Initialized(false)
{
    //initialization of free_units is done with first loadTexture call because we need to make sure that the opengl context is already created
}

TextureManager::~TextureManager()
{

}

bool TextureManager::loadTexture(std::string sTextureName, std::string sFilename, bool bAlreadyGammaCorrected, GLint iTarget /*= GL_TEXTURE_2D*/)
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
			this->free_units.push_back(a);

		Logger::debug() << puffer2 << " texture units available" << Logger::endl;

		m_bDevIL_Initialized = true;

		Logger::debug() << "DevIL initialized" << Logger::endl;
	}

	Logger::debug() << "try to load texture from \"" << sFilename << "\"" << Logger::endl;

	ILuint DevILID;				// index für DevIL Textur

	ilGenImages(1,&DevILID);			// generieren von IL ID für Texturen
	ilBindImage(DevILID);			// bestimmten Texturindex als aktell setzen

	if (!ilLoadImage (sFilename.data()))		// laden der Textur
	{
		ilDeleteImages(1,&DevILID);		// bei Fehlschlag wieder Index freigeben

		Logger::error() << "Could not find texture file " << sFilename << ", loading failed." << Logger::endl;

		return false;
	}
	else
	{
		long h, w, bpp, f;
		unsigned char *texdata=0;
		GLuint OpenGLID;

		w=ilGetInteger(IL_IMAGE_WIDTH);		    // Breite des Bildes holen
		h=ilGetInteger(IL_IMAGE_HEIGHT);	    // Höhe des Bildes holen
		bpp=ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // Farbtiefe des Bildes
		f=ilGetInteger(IL_IMAGE_FORMAT);	    // Format des Bildes z.B. RGB RGBA BGR BGRA usw.
		texdata=ilGetData();			    // Zeiger auf Bilddaten holen

		glGetError();				    // clear error state
		glGenTextures(1,&OpenGLID);		    // ask for opengl texture-id (returns first free id)
		glBindTexture(iTarget, OpenGLID);

		if (bAlreadyGammaCorrected)
			glTexImage2D(iTarget, 0, GL_SRGB8_ALPHA8, w, h, 0, f, GL_UNSIGNED_BYTE, texdata);
		else
			glTexImage2D(iTarget, 0, GL_RGBA, w, h, 0, f, GL_UNSIGNED_BYTE, texdata);


		//activate mipmapping
		glTexParameteri(iTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(iTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		float maximumAnisotropy;
		//get maximum ansitropic filtering value
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);

		//activate 4x ansitropic filtering if possible
		if (maximumAnisotropy >= 4.0)
			glTexParameterf(iTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		else
		    Logger::error() << "4x Ansitropic filtering not available by graphic hardware" << Logger::endl;

		glGenerateMipmap(iTarget);  //Generate mipmaps now!!!

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		    Logger::error() << "glGetError: " << translateGLerror(error) << Logger::endl;

		this->m_mTextureIDs[sTextureName] = OpenGLID;
		this->m_mTextureTargets[sTextureName] = iTarget;
		this->textures_in_units[sTextureName] = -1;

		Logger::debug() << "texture from " << sFilename << " loaded to intern name \"" << sTextureName << "\"" << Logger::endl;
		return true;
	}
}


GLuint TextureManager::useTexture(std::string sTextureName)
{
    int old_size = this->m_mTextureIDs.size();
    GLuint texture_id = this->m_mTextureIDs[sTextureName];

    assert (m_mTextureLocks.find(texture_id) == m_mTextureLocks.end() || m_mTextureLocks[texture_id] == false);

    m_mTextureLocks[texture_id] = true;

    //if the size of the texture-container NOW is creater than old_size,
    //the [] operator created a new entry in the map, which means that an entry with the given name did not exist
    if (old_size != this->m_mTextureIDs.size())
	Logger::fatal() << "Texture \"" << sTextureName << "\" could not be found (not loaded?)" << Logger::endl;
    else
    {
	if (this->free_units.size() == 0)
	    Logger::error() << "no free texture unit to use texture \"" << sTextureName << "\"" << Logger::endl;
	else
	{
	    if (this->textures_in_units[sTextureName] != -1)		    //if this texture is currently stored in an unit, return the id of the unit
		return this->textures_in_units[sTextureName];
	    else						    //else find a free unit and load the texture in it
	    {

#ifdef PREVENT_REBINDING_TEXTURE
		//this block loops through all units and looks if on of the units is marked as free, but has the wanted texture loaded
		//then we can reuse this unit without reloading the texture in it
		//but we must erase the unit from the free_units-list
		for (int a=0; a < this->m_iMaxTextureUnits; a++)
		    if (this->unit_has_texture[a] == texture_id)
		    {
			for (std::list<GLuint>::iterator it1=free_units.begin(); it1!=free_units.end(); ++it1)
			    if (*it1 == a)
			    {
				this->free_units.erase(it1);
				break;
			    }
			this->textures_in_units[sTextureName] = a;
			return a;
		    }
#endif

		GLuint free_unit = this->getFreeUnit();		    //remove unit from free_units queue
		this->textures_in_units[sTextureName] = free_unit;	    //make a note that this texture now is stored in a unit

#ifdef PREVENT_REBINDING_TEXTURE
		this->unit_has_texture[free_unit] = texture_id;	    //mark that the texture (texture_id) is stored in this unit, to prevent rebinding the texture in another unit
#endif

		assert (m_mTextureTargets.find(sTextureName) != m_mTextureTargets.end());

		// get target
		GLint iTarget = m_mTextureTargets[sTextureName];

		glActiveTexture(GL_TEXTURE0 + free_unit);	    //activate given texture unit
		glBindTexture(iTarget, texture_id);		    //load texture in this unit

		return free_unit;
	    }
	}
    }
}

void TextureManager::unuseTexture(std::string sTextureName)
{
    GLint used_unit = this->textures_in_units[sTextureName];	//which unit was used by given texture

    if (used_unit != -1)
    {
	//cout << "release unit: " << used_unit << endl;
	this->releaseUnit(used_unit);
	this->textures_in_units[sTextureName] = -1;			//mark texture as "not stored in any unit"

	m_mTextureLocks[m_mTextureIDs[sTextureName]] = false;
    }
    else
	Logger::error() << "not possible to release texture unit used by texture \"" << sTextureName << "\", texture was not loaded in any unit!" << Logger::endl;
}

TextureManager *TextureManager::instance()
{
    static TextureManager singelton_instance;
    return &singelton_instance;
}

GLuint TextureManager::getFreeUnit()
{
    GLuint free_unit = this->free_units.front();	    //get first free unit
    this->free_units.pop_front();			    //remove unit from free_units queue

#ifdef PREVENT_REBINDING_TEXTURE
    this->unit_has_texture[free_unit] = -1;		    //mark that the returned unit has no loaded texture in it
#endif

    return free_unit;
}

void TextureManager::releaseUnit(GLuint nUnit)
{
    this->free_units.push_back(nUnit);
}

void TextureManager::registerManualTexture(std::string sTextureName, GLuint nTextureID, GLenum eTarget /* = GL_TEXTURE_2D */)
{
    Logger::debug() << "Registered manual texture \"" << sTextureName << "\" with id " << nTextureID << Logger::endl;

    int iOldSize = m_mTextureIDs.size();

    m_mTextureIDs[sTextureName] = nTextureID;
    m_mTextureTargets[sTextureName] = eTarget;

    this->textures_in_units[sTextureName] = -1;

    assert (m_mTextureIDs.size() != iOldSize);
}

bool TextureManager::isTextureRegistered(std::string sTextureName, GLuint &rnTextureID)
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

bool TextureManager::isTextureInUse(GLuint nTextureID)
{
    std::string sTextureName;

    for (std::map<std::string, GLuint>::iterator iter = m_mTextureIDs.begin(); iter != m_mTextureIDs.end(); iter++)
	if (iter->second == nTextureID)
	{
	    sTextureName = iter->first;
	    break;
	}

    assert (sTextureName.empty() == false);
    assert (textures_in_units.find(sTextureName) != textures_in_units.end());

    return (textures_in_units[sTextureName] != -1);
}

void TextureManager::lockTextureID(GLuint nTextureID)
{
    m_mTextureLocks[nTextureID] = true;
}

void TextureManager::unlockTextureID(GLuint nTextureID)
{
    m_mTextureLocks[nTextureID] = false;
}

GLuint TextureManager::CreateSampler(std::string sTextureName, GLenum eTarget, GLint iInternalTextureFormat, GLint iWidth, GLint iHeight, GLint iClampMode, GLint iFiltering)
{
    GLuint nTextureID;

    GLuint nAssignedUnit = getFreeUnit();

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
    releaseUnit(nAssignedUnit);

    // register texture
    registerManualTexture(sTextureName, nTextureID, eTarget);

    // return open gl texture id
    return nTextureID;
}

GLint TextureManager::GetTextureTarget(std::string sTextureName)
{
    int iOldSize = m_mTextureTargets.size();

    GLint iTarget = m_mTextureTargets[sTextureName];

    // check that size has not changed (else the entry did not exist before)
    assert (m_mTextureTargets.size() == iOldSize);

    return iTarget;
}

bool TextureManager::isTextureLocked(GLuint texture_id)
{
    return !(m_mTextureLocks.find(texture_id) == m_mTextureLocks.end() || m_mTextureLocks[texture_id] == false);
}
