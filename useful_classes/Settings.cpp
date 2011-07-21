#include "Settings.h"
#include "Logger.h"

#include <iostream>

Settings::Settings()
    : m_rMainGroup("#")
{

}

Settings::~Settings()
{

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
    std::map<std::string, TSettingsGroup *>::iterator iter = m_mGroups.find(sName);

    bool bGroupExistsAlready = (iter != m_mGroups.end());

    TSettingsGroup *pGroup = NULL;

    if (bGroupExistsAlready == false)
	m_mGroups[sName] = pGroup = new TSettingsGroup(sName);
    else
	pGroup = iter->second;

    assert (pGroup != NULL);

    return pGroup;
}

Settings::TSettingsGroup::TSettingsGroup(std::string sName)
    : m_sName(sName)
{
}

bool Settings::TSettingsGroup::ItlTestIfNameAlreadyUsed(std::string sName)
{
    /// check if in any container this name is already used

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
