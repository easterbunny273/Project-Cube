#include "Gamelogic/Level.h"

#include <iostream>
using namespace std;

#include "Logger.h"


Level::Level()
{
	m_nNumCubes = 0;
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
	TiXmlElement *pElement;
	
	bool bOk = true;

	if(pRootElement == NULL)
	{
		bOk = false;
	}

	if(bOk)
	{
		std::string sRootElementName(pRootElement->Value());
		
		assert (sRootElementName.length() > 0);

		pRootElement->QueryUnsignedAttribute("id", &m_nLevelID);
		pRootElement->QueryIntAttribute("numcubes", &m_nNumCubes);
		pRootElement->QueryStringAttribute("name", &m_sLevelName);

		Logger::debug() << m_nLevelID << " " << m_nNumCubes << " " << m_sLevelName << Logger::endl;
		
		TiXmlElement* pChildElement = pRootElement->FirstChildElement();
		
		do
		{
			std::string sTagName(pChildElement->Value());
			if(sTagName.compare("group")!=0)
				bOk = false;

			bOk = itlReadGroupFromXML(pChildElement);
		}
		while(bOk && (pChildElement = pChildElement->NextSiblingElement()));
		


	}

	return bOk;

}

bool Level::WriteToXMLString(std::string &sFilename)
{
	return false;
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

bool Level::itlReadGroupFromXML(TiXmlElement *pGroup)
{
	std::string sName;

	pGroup->QueryStringAttribute("name", &sName);

	if(sName.compare("cubes"))
	{
		return itlLoadCubesFromXML(pGroup);
	}
	else
	{
		//TODO other group loading
	}
	return true;
	
}

bool Level::itlLoadCubesFromXML(TiXmlElement *pCubeGroup)
{
	bool bOk = true;

	TiXmlElement* pCubeElement = pCubeGroup->FirstChildElement();

	do
	{
		assert (pCubeElement != NULL);

		std::string sTagName(pCubeElement->Value());

		if(sTagName.compare("cube")!=0)
			return false;

		unsigned int iCubeID;
		int iX;
		int iY;
		int iZ;

		pCubeElement->QueryUnsignedAttribute("id", &iCubeID);
		pCubeElement->QueryIntAttribute("x", &iX);
		pCubeElement->QueryIntAttribute("y", &iY);
		pCubeElement->QueryIntAttribute("z", &iZ);

		Cube cube(iCubeID, iX, iY, iZ);

		TiXmlElement* pGridElement = pCubeElement->FirstChildElement();

		do
		{
			assert(pGridElement != NULL);

			std::string sTagName(pGridElement->Value());
			if(sTagName.compare("grid")!=0)
				return false;

			std::string sGridPos;
			pGridElement->QueryStringAttribute("pos", &sGridPos);

			Grid grid;

			if(sGridPos.compare("Xplus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetXplus(grid);
			}
			else if(sGridPos.compare("Xminus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetXminus(grid);
			}
			else if(sGridPos.compare("Yplus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetYplus(grid);
			}
			else if(sGridPos.compare("Yminus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetYminus(grid);
			}
			else if(sGridPos.compare("Zplus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetZplus(grid);
			}
			else if(sGridPos.compare("Zminus"))
			{
				bOk = itlLoadGridFromXML(pGridElement, grid);
				cube.SetZminus(grid);
			}
			else
			{
				bOk = false;
			}

		}
		while(bOk && (pGridElement = pGridElement->NextSiblingElement()));
		
		if(bOk)
			itlAddCube(cube);

	}
	while(bOk && (pCubeElement = pCubeElement->NextSiblingElement()));

	return bOk;

}

bool Level::itlLoadGridFromXML(TiXmlElement* pGridElement, Grid& grid)
{
	bool bOk = true;

	TiXmlElement* pDoorElement = pGridElement->FirstChildElement();

	do
	{
		assert(pDoorElement != NULL);

		std::string sTagName(pDoorElement->Value());
		if(sTagName.compare("door")!=0)
			return false;

		int iX;
		int iY;

		pDoorElement->QueryIntAttribute("x", &iX);
		pDoorElement->QueryIntAttribute("y", &iY);

		grid.AddDoor(iX, iY);
	}
	while(bOk && (pDoorElement = pDoorElement->NextSiblingElement()));

	return bOk;
}

void Level::itlAddCube(Cube& cube)
{
	m_Cubes.push_back(cube);
	m_nNumCubes++;
}