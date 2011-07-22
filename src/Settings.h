#include <map>
#include <string>
#include "tinyxml.h"

class Settings
{
public:
	static Settings& instance();
	void blabla();


private:
	Settings();
	~Settings();
	Settings(const Settings&);

	std::map<std::string, int>		m_mIntEntries;
	std::map<std::string, std::string>	m_mStringEntries;
	std::map<std::string, bool>		m_mBoolEntries;
	std::map<std::string, float>		m_mFloatEntries;
	//std::map<std::string, glm::vec4>	m_mVecEntries;

	void itlLoadSettings(std::string sFile);
	void itlLoadValue(TiXmlHandle handle, std::string sPath);
};