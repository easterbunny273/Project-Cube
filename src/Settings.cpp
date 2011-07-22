#define TIXML_USE_STL

#include "Settings.h"
#include <iostream>
using namespace std;


Settings::Settings()
{
	itlLoadSettings("./config/config.xml");
}

Settings::~Settings()
{
}

Settings& Settings::instance()
{
	static Settings instance;
	return instance;
}

void blabla()
{
	cout << "blabla" << endl;
}

void Settings::itlLoadSettings(std::string sFile)
{
	TiXmlDocument doc(sFile);
	if(!doc.LoadFile())
	{
		std::cout << "Could not load file: " << sFile << std::endl;
		return;
	}

	TiXmlHandle hDoc(&doc);
	itlLoadValue(hDoc, std::string(""));
}

void Settings::itlLoadValue(TiXmlHandle handle, std::string sPath)
{
	TiXmlElement* pElement = handle.FirstChildElement().Element();
	
	
	for(pElement; pElement; pElement = pElement->NextSiblingElement())
	{
		if(pElement->Attribute("type"))
		{
			std::string sMapPath = sPath+pElement->Value();
			std::string type;
			pElement->QueryStringAttribute("type", &type);
			if(type == "int")
			{
				int value;
				pElement->QueryIntAttribute("value", &value);
				m_mIntEntries[sMapPath] = value;
				cout << sMapPath << " " << value << endl;//debug
			}
			else if(type == "string")
			{
				std::string value;
				pElement->QueryStringAttribute("value", &value);
				m_mStringEntries[sMapPath] = value;
				cout << sMapPath << " " << value << endl;//debug
			}
			else if(type == "bool")
			{
				bool value;
				pElement->QueryBoolAttribute("value", &value);
				m_mBoolEntries[sMapPath] = value;
				cout << sMapPath << " " << value << endl;//debug
			}
			else if(type == "float")
			{
				float value;
				pElement->QueryFloatAttribute("value", &value);
				m_mFloatEntries[sMapPath] = value;
				cout << sMapPath << " " << value << endl;//debug
			}
		}
		else
		{
			sPath.append(pElement->Value());
			sPath.append(".");
			itlLoadValue(TiXmlHandle(pElement), sPath);
		}
	}
}

