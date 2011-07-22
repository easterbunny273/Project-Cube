#include "Settings.h"
#include "Logger.h"

#include <sstream>
#include <iostream>
#include <fstream>

#define TIXML_USE_STL

#include "tinyxml.h"
#include "tinystr.h"

Settings::Settings()
{

}

Settings::~Settings()
{
    // clear all groups to destroy them automatically (we are using shared_ptr)
    Clear();
}

Settings * Settings::instance()
{
    // create instance, if not done yet
    static Settings pInstance;

    return &pInstance;
}

void Settings::TSettingsGroup::SetValue(std::string sName, int iValue)
{
    std::map<std::string, int>::iterator iter = m_mIntEntries.find(sName);

    if (iter == m_mIntEntries.end())
	if (ItlTestIfNameAlreadyUsed(sName))
	    Logger::fatal() << "The key \"" << sName << "\" already exists in this settings group but has a different value type. Check your code." << Logger::endl;

    m_mIntEntries[sName] = iValue;
}

void Settings::TSettingsGroup::SetValue(std::string sName, float fValue)
{
    std::map<std::string, float>::iterator iter = m_mFloatEntries.find(sName);

    if (iter == m_mFloatEntries.end())
	if (ItlTestIfNameAlreadyUsed(sName))
	    Logger::fatal() << "The key \"" << sName << "\" already exists in this settings group but has a different value type. Check your code." << Logger::endl;

    m_mFloatEntries[sName] = fValue;
}

void Settings::TSettingsGroup::SetValue(std::string sName, std::string sValue)
{
    std::map<std::string, std::string>::iterator iter = m_mStringEntries.find(sName);

    if (iter == m_mStringEntries.end())
	if (ItlTestIfNameAlreadyUsed(sName))
	    Logger::fatal() << "The key \"" << sName << "\" already exists in this settings group but has a different value type. Check your code." << Logger::endl;

    m_mStringEntries[sName] = sValue;
}

void Settings::TSettingsGroup::SetValue(std::string sName, const char * szValue)
{
    std::string sValue(szValue);

    SetValue(sName, sValue);
}

void Settings::TSettingsGroup::SetValue(std::string sName, bool bValue)
{
    std::map<std::string, bool>::iterator iter = m_mBoolEntries.find(sName);

    if (iter == m_mBoolEntries.end())
	if (ItlTestIfNameAlreadyUsed(sName))
	    Logger::fatal() << "The key \"" << sName << "\" already exists in this settings group but has a different value type. Check your code." << Logger::endl;

    m_mBoolEntries[sName] = bValue;
}

void Settings::TSettingsGroup::SetValue(std::string sName, glm::vec4 vValue)
{
    std::map<std::string, glm::vec4>::iterator iter = m_mVecEntries.find(sName);

    if (iter == m_mVecEntries.end())
	if (ItlTestIfNameAlreadyUsed(sName))
	    Logger::fatal() << "The key \"" << sName << "\" already exists in this settings group but has a different value type. Check your code." << Logger::endl;

    m_mVecEntries[sName] = vValue;
}

void Settings::TSettingsGroup::SetValue(std::string sName, glm::vec2 vValue)
{
    SetValue(sName, glm::vec4(vValue.x, vValue.y, 0.0, 0.0));
}

void Settings::TSettingsGroup::SetValue(std::string sName, glm::vec3 vValue)
{
    SetValue(sName, glm::vec4(vValue.x, vValue.y, vValue.z, 0.0));
}

bool Settings::TSettingsGroup::GetValue(std::string sName, int &iValue)
{
    std::map<std::string, int>::iterator iter = m_mIntEntries.find(sName);

    bool bFoundValue = (iter != m_mIntEntries.end());

    if (bFoundValue)
	iValue = iter->second;

    return bFoundValue;
}

bool Settings::TSettingsGroup::GetValue(std::string sName, float &fValue)
{
    std::map<std::string, float>::iterator iter = m_mFloatEntries.find(sName);

    bool bFoundValue = (iter != m_mFloatEntries.end());

    if (bFoundValue)
	fValue = iter->second;

    return bFoundValue;
}

bool Settings::TSettingsGroup::GetValue(std::string sName, std::string &sValue)
{
    std::map<std::string, std::string>::iterator iter = m_mStringEntries.find(sName);

    bool bFoundValue = (iter != m_mStringEntries.end());

    if (bFoundValue)
	sValue = iter->second;

    return bFoundValue;
}

bool Settings::TSettingsGroup::GetValue(std::string sName, bool &bValue)
{
    std::map<std::string, bool>::iterator iter = m_mBoolEntries.find(sName);

    bool bFoundValue = (iter != m_mBoolEntries.end());

    if (bFoundValue)
	bValue = iter->second;

    return bFoundValue;
}

bool Settings::TSettingsGroup::GetValue(std::string sName, glm::vec4 &vValue)
{
    std::map<std::string, glm::vec4>::iterator iter = m_mVecEntries.find(sName);

    bool bFoundValue = (iter != m_mVecEntries.end());

    if (bFoundValue)
	vValue = iter->second;

    return bFoundValue;
}


bool Settings::TSettingsGroup::GetValue(std::string sName, glm::vec3 &vValue)
{
    glm::vec4 vValueAsVec4;

    bool bFoundValue = GetValue(sName, vValueAsVec4);

    if (bFoundValue)
    {
	vValue.x = vValueAsVec4.x;
	vValue.y = vValueAsVec4.y;
	vValue.z = vValueAsVec4.z;
    }

    return bFoundValue;
}

bool Settings::TSettingsGroup::GetValue(std::string sName, glm::vec2 &vValue)
{
    glm::vec4 vValueAsVec4;

    bool bFoundValue = GetValue(sName, vValueAsVec4);

    if (bFoundValue)
    {
	vValue.x = vValueAsVec4.x;
	vValue.y = vValueAsVec4.y;
    }

    return bFoundValue;
}

Settings::TSettingsGroup * Settings::TSettingsGroup::GetGroup(std::string sName)
{
    std::map<std::string, std::shared_ptr<TSettingsGroup> >::iterator iter = m_mGroups.find(sName);

    bool bGroupExistsAlready = (iter != m_mGroups.end());

    TSettingsGroup *pGroup = NULL;

    if (bGroupExistsAlready == false)
    {
        pGroup = new TSettingsGroup(sName);
        m_mGroups[sName] = std::shared_ptr<TSettingsGroup>(pGroup);
    }
    else
        pGroup = &(*(iter->second));

    assert (pGroup != NULL);

    return pGroup;
}

Settings::TSettingsGroup::TSettingsGroup(std::string sName)
    : m_sName(sName)
{
}

bool Settings::TSettingsGroup::ItlTestIfNameAlreadyUsed(std::string sName)
{
    // check if in any container this name is already used

    if (m_mIntEntries.find(sName) != m_mIntEntries.end())
	return true;

    if (m_mFloatEntries.find(sName) != m_mFloatEntries.end())
	return true;

    if (m_mBoolEntries.find(sName) != m_mBoolEntries.end())
	return true;

    if (m_mStringEntries.find(sName) != m_mStringEntries.end())
	return true;

    if (m_mVecEntries.find(sName) != m_mVecEntries.end())
	return true;

    return false;
}

bool Settings::WriteToXMLString(std::string &rsString)
{
    // create output stream
    std::ostringstream ssStream;

    // open settings node
    ssStream << "<settings>";

    // write root group as xml (writes recursively all groups)
    ItlGetMainGroup()->ItlWriteGroupAsXMLToStringStream(ssStream);

    // close settings node
    ssStream << "</settings>";

    // get string from stream
    rsString = ssStream.str();
}

bool Settings::ReadFromXMLString(std::string sString)
{
    // clear settings
    Clear();

    // create tiny-xml-document object
    TiXmlDocument document;

    // parse given string and create DOM-structure
    document.Parse(sString.c_str());

    // get handle
    TiXmlHandle hHandle(&document);

    // get root element
    TiXmlElement *pRootElement = hHandle.FirstChildElement().Element();

    // bool if everything is fine
    bool bOk = true;

    if (pRootElement == NULL)
        bOk = false;

    if (bOk)
    {
        // get name of root element
        std::string sRootElementName(pRootElement->Value());

        assert (sRootElementName.length() > 0);

        // if root element is a <settings> node
        if (sRootElementName.compare("settings") == 0)
        {
            // get first child element
            TiXmlElement* pElement = pRootElement->FirstChildElement();

            do
            {
                // make sure that we have a valid pointer
                assert (pElement != NULL);

                // get tag name
                std::string sTagName(pElement->Value());

                // if it is a <group> tag ...
                if (sTagName.compare("group")==0)
                {
                    std::string sName;

                    // get name of value
                    pElement->QueryStringAttribute("name", &sName);

                    // if name is "#", it is the main group of a settings structure and everything is fine,
                    // start reading groups recursively
                    if (sName.compare("#") == 0)
                        ItlGetMainGroup()->ItlReadGroupFromXML(pElement);
                    else
                        // else, something went wrong
                        bOk = false;
                }
                else
                    // something went wrong, we expected a <group> tag but found an other tag
                    bOk = false;
            }
            while (bOk && (pElement = pElement->NextSiblingElement()));
        }
        else
            // something went wrong, root tag is not <settings> as we expected
            bOk = false;
    }

    // return if loading was successfull
    return bOk;
}

void Settings::TSettingsGroup::ItlWriteGroupAsXMLToStringStream(std::ostringstream &rssStream)
{
    // open group tag
    rssStream << "<group name=\"" << m_sName << "\">";

    // write child groups
    for (auto iter=m_mGroups.begin(); iter != m_mGroups.end(); iter++)
    {
        // get child group
        TSettingsGroup *pChildGroup = &(*(iter->second));

        // make sure we have a valid pointer
        assert (pChildGroup != NULL);

        // write child group to stream
        pChildGroup->ItlWriteGroupAsXMLToStringStream(rssStream);
    }

    // write int values
   for (auto iter=m_mIntEntries.begin(); iter != m_mIntEntries.end(); iter++)
    {
        rssStream << "<value name=\"" << iter->first << "\" type=\"int\">";
        rssStream << iter->second;
        rssStream << "</value>";
    }

   // write float values
  for (auto iter=m_mFloatEntries.begin(); iter != m_mFloatEntries.end(); iter++)
   {
       rssStream << "<value name=\"" << iter->first << "\" type=\"float\">";
       rssStream << iter->second;
       rssStream << "</value>";
   }

  // write bool values
 for (auto iter=m_mBoolEntries.begin(); iter != m_mBoolEntries.end(); iter++)
  {
      rssStream << "<value name=\"" << iter->first << "\" type=\"boolean\">";
      rssStream << iter->second;
      rssStream << "</value>";
  }

 // write vec values
    for (auto iter=m_mVecEntries.begin(); iter != m_mVecEntries.end(); iter++)
    {
        rssStream << "<value name=\"" << iter->first << "\" type=\"glm_vec\">";
        rssStream << iter->second.x << " " << iter->second.y << " " << iter->second.z << " " << iter->second.w;
        rssStream << "</value>";
    }

   // write string values
  for (auto iter=m_mStringEntries.begin(); iter != m_mStringEntries.end(); iter++)
   {
       rssStream << "<value name=\"" << iter->first << "\" type=\"string\">";
       rssStream << iter->second;
       rssStream << "</value>";
   }

  // close group tag
  rssStream << "</group>";
}

bool Settings::TSettingsGroup::ItlReadGroupFromXML(TiXmlElement *pElement)
{
    // flag to show if everything was fine
    bool bOk = true;

    // get first child element
    TiXmlElement* pChildElement = pElement->FirstChildElement();

    do
    {
        // make sure that we got a valid pointer
        assert (pChildElement != NULL);

        // get tag name
        std::string sTagName(pChildElement->Value());

        // if it is a <group> tag ...
        if (sTagName.compare("group")==0)
        {
            std::string sName;

            // get name of group
            pChildElement->QueryStringAttribute("name", &sName);

            // check if group has a valid name
            bOk = (sName.length() > 0);

            assert (bOk);

            // if everything was fine, read group from xml element
            if (bOk)
                bOk = GetGroup(sName)->ItlReadGroupFromXML(pChildElement);

            assert (bOk);

        }
        // else, if it is a <value> tag ...
        else if (sTagName.compare("value")==0)
        {
            std::string sName;
            std::string sType;

            // get name attribute
            pChildElement->QueryStringAttribute("name", &sName);

            // get type attribute
            pChildElement->QueryStringAttribute("type", &sType);

            // check if the values are valid
            bOk &= (sName.length() > 0);
            bOk &= (sType.length() > 0);

            assert (bOk);

            // get inner value of the tag (<xy>I_MEAN_THIS_TEXT</xy>)
            const char *szValue = pChildElement->GetText();

            // make sure that we got a valid pointer
            if (szValue != NULL)
            {
                // convert to stl string
                std::string sValue(szValue);

                // parse value (and store in group)
                bOk = ItlParseValue(sName, sType, sValue);
            }
            else // something went wrong
                bOk = false;

            assert (bOk);

        }
        else
        {
            assert (!"unknown entity");
            bOk = false;
        }
    }
    while (bOk && (pChildElement = pChildElement->NextSiblingElement()));

    return bOk;
}

Settings::TSettingsGroup::~TSettingsGroup()
{

}

void Settings::Clear()
{
    // clear main group

    m_spMainGroup.reset();
}

void Settings::TSettingsGroup::Clear()
{
    // clear all maps

    m_mGroups.clear();
    m_mIntEntries.clear();
    m_mStringEntries.clear();
    m_mBoolEntries.clear();
    m_mVecEntries.clear();
    m_mFloatEntries.clear();
}

bool Settings::TSettingsGroup::ItlParseValue(std::string sName, std::string sType, std::string sValue)
{
    // make sure we got valid parameters (sValue may be empty!)
    assert (sName.length() > 0);
    assert (sType.length() > 0);

    // flag if everything was fine so far
    bool bOk = true;

    if (sType.compare("string")==0)
    {
        // store string value in map
        m_mStringEntries[sName] = sValue;
    }
    else if (sType.compare("int")==0)
    {
        // convert sValue to integer
        // and store value in map

        std::stringstream ssBuffer;
        int iValue;

        ssBuffer << sValue;
        ssBuffer >> iValue;

        m_mIntEntries[sName] = iValue;
    }
    else if (sType.compare("float")==0)
    {
        // convert sValue to float
        // and store value in map

        std::stringstream ssBuffer;
        float fValue;

        ssBuffer << sValue;
        ssBuffer >> fValue;

        m_mFloatEntries[sName] = fValue;
    }
    else if (sType.compare("boolean")==0)
    {
        // convert sValue to bool
        // and store value in map

        std::stringstream ssBuffer;
        bool bValue;

        ssBuffer << sValue;
        ssBuffer >> bValue;

        m_mBoolEntries[sName] = bValue;
    }
    else if (sType.compare("glm_vec")==0)
    {
        // convert sValue to glm::vector
        // and store value in map

        std::stringstream ssBuffer;
        float fX, fY, fZ, fW;

        ssBuffer << sValue;

        ssBuffer >> fX;
        ssBuffer >> fY;
        ssBuffer >> fZ;
        ssBuffer >> fW;

        m_mVecEntries[sName] = glm::vec4(fX, fY, fZ, fW);
    }
    else // unknow type
        bOk = false;

    return bOk;
}

void Settings::StoreSettingsAsXMLFile(std::string sFilename)
{
    assert (sFilename.length() > 0);

    // fill xml document from xml string
    std::string sXMLString;

    // write settings in xml string
    WriteToXMLString(sXMLString);

    // create TinyXMLDocument
    TiXmlDocument document;

    // parse string and create dom structure
    document.Parse(sXMLString.c_str());

    // save dom structure to file
    bool bOk = document.SaveFile(sFilename);

    // print error message if not successfull
    if (!bOk)
        Logger::error() << "Saving settings to file \"" << sFilename << "\" failed" << Logger::endl;
}

bool Settings::RestoreSettingsFromXMLFile(std::string sFilename)
{
    // file handle, open file
    std::ifstream file(sFilename);

    // check if file is open
    bool bOk = file.is_open();

    // if file could be opened, read content
    if (bOk)
    {
        // string to store content
        std::string sFileContent;

        // as long as there is anything in the stream,
        // read it and append it to sFileContent
        while (!file.eof())
        {
            std::string sWord;

            file >> sWord;

            sFileContent.append(sWord);
            sFileContent.append(" ");
        }

        assert (sFileContent.length() > 0);

        bOk = ReadFromXMLString(sFileContent);
    }

    // print error message if loading was not successfull
    if (!bOk)
        Logger::error() << "Loading settings from file \"" << sFilename << "\" failed" << Logger::endl;
}

Settings::TSettingsGroup * Settings::ItlGetMainGroup()
{
    // if shared pointer points to NULL, create new main group
    if (m_spMainGroup.get() == NULL)
    {
        m_spMainGroup = std::shared_ptr<TSettingsGroup>(new TSettingsGroup("#"));
    }

    // return raw pointer to main group (because only used internal and not for delete)
    return m_spMainGroup.get();
}

Settings::TSettingsGroup * Settings::GetGroup(std::string sName)
{
    // get main group
    TSettingsGroup *pMainGroup = ItlGetMainGroup();

    // make sure ptr is valid
    assert (pMainGroup != NULL);

    // redirect call to main group
    return pMainGroup->GetGroup(sName);
}
