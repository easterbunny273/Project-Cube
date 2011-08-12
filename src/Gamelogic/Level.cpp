#include "Gamelogic/Level.h"

#include <iostream>
using namespace std;

#include "Logger.h"
#include "tinyxml.h"
#include "tinystr.h"

Level::Level(std::string sFilename)
{
	m_nNumCubes = 0;
	ReadFromXMLString(sFilename);
}

Level::~Level()
{
}

bool Level::ReadFromXMLString(std::string sFilename)
{
	Clear();

	TiXmlDocument document(sFilename);
	if(!document.LoadFile())
	{
		Logger::debug() << "XML-file " << sFilename << " could not be loaded!" << Logger::endl;
		return false;
	}
	
	TiXmlHandle hHandle(&document);
	
	TiXmlElement *pRootElement = hHandle.FirstChildElement().Element();
	
	bool bOk = true;

	if(pRootElement == NULL)
	{
		bOk = false;
	}

	if(bOk)
	{
		std::string sRootElementName(pRootElement->Value());
		
		assert (sRootElementName.length() > 0);
		
	}

	return bOk;

}

bool Level::WriteToXMLString(std::string &sFilename)
{
	return false;
}

void Level::itlAddCube(Cube& cube)
{
	m_Cubes.push_back(cube);
	m_nNumCubes++;
}

bool Level::RotateX(const int iFactor)
{
	return false;
}
bool Level::RotateY(const int iFactor)
{
	return false;
}
bool Level::RotateZ(const int iFactor)
{
	return false;
}

void Level::Clear()
{
	m_Cubes.clear();
	m_nNumCubes = 0;
}