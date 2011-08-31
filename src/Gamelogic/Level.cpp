#include "Gamelogic/Level.h"

#include <iostream>
using namespace std;

#include "Logger.h"


Level::Level()
{
	m_iNumCubes = 0;
}

Level::~Level()
{
}

/*#############################################################
*################## PUBLIC METHODS ###########################
#############################################################*/

unsigned int Level::GetLevelID()
{
	return m_nLevelID;
}

std::string Level::GetLevelName()
{
	return m_sLevelName;
}

int Level::GetNumCubes()
{
	return m_iNumCubes;
}

void Level::Clear()
{
	m_Cubes.clear();
	m_iNumCubes = 0;
	m_nLevelID = 0;
	m_sLevelName = "empty level";
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

bool Level::ReadFromXMLString(std::string sFilename)
{
	Clear();

	TiXmlDocument document(sFilename);
	if(!document.LoadFile())
	{
		Logger::debug() << "### XML-file " << sFilename << " could not be loaded!" << Logger::endl;
		return false;
	}
	
	TiXmlHandle hHandle(&document);
	
	TiXmlElement *pRootElement = hHandle.FirstChildElement().Element();
	TiXmlElement *pElement;
	
	bool bOk = true;

	if(pRootElement == NULL)
	{
		Logger::debug() << "pRootElement is NULL" << Logger::endl;
		bOk = false;
	}

	if(bOk)
	{
		std::string sRootElementName(pRootElement->Value());
		
		assert (sRootElementName.length() > 0);

		pRootElement->QueryUnsignedAttribute("id", &m_nLevelID);
		pRootElement->QueryStringAttribute("name", &m_sLevelName);

		Logger::debug() << "### Loading level from XML: id=" << m_nLevelID << " name=" << m_sLevelName << Logger::endl;
		
		TiXmlElement* pChildElement = pRootElement->FirstChildElement();
		
		do
		{
			std::string sTagName(pChildElement->Value());
			if(sTagName.compare("group")!=0)
			{
				Logger::debug() << m_sLevelName << ": line " << pChildElement->Row() << ": Tagname should be a group, instead is: " << sTagName << Logger::endl;
				bOk = false;
			}
			else
			{
				bOk = itlReadGroupFromXML(pChildElement);
			}
		}
		while(bOk && (pChildElement = pChildElement->NextSiblingElement()));
	}

	if(bOk == false)
		Logger::debug() << m_sLevelName << ": COULD NOT LOAD LEVEL" << Logger::endl;
	else
		Logger::debug() << m_sLevelName << ": LEVEL LOADING SUCCESSFUL" << Logger::endl;

	return bOk;
}

bool Level::WriteToXMLString(std::string &sFilename)
{
	return false;
}



/*#############################################################
*################## PRIVATE METHODS ##########################
#############################################################*/

bool Level::itlReadGroupFromXML(TiXmlElement *pGroup)
{
	std::string sName;
	pGroup->QueryStringAttribute("name", &sName);

	Logger::debug() << m_sLevelName << ": line " << pGroup->Row() << ": Loading group: " << sName << Logger::endl;

	if(sName.compare("cubes")==0)
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
	Logger::debug() << m_sLevelName << ": line " << pCubeGroup->Row() << ": Loading cubegroup" << Logger::endl;

	bool bOk = true;

	TiXmlElement* pCubeElement = pCubeGroup->FirstChildElement();

	std::string sName;

	pCubeGroup->QueryStringAttribute("name", &sName);

	do
	{
		assert (pCubeElement != NULL);

		std::string sTagName(pCubeElement->Value());

		if(sTagName.compare("cube")!=0)
		{
			Logger::debug() << m_sLevelName << ": line " << pCubeElement->Row() << ": Tagname should be a cube, instead is: " << sTagName << Logger::endl;
			return false;
		}

		Logger::debug() << m_sLevelName << ": line " << pCubeElement->Row() << ": Loading cube" << Logger::endl;

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
			{
				Logger::debug() << m_sLevelName << ": line " << pGridElement->Row() << ": Tagname should be 'grid', instead is: " << sTagName << Logger::endl;
				bOk = false;
			}
			else
			{
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
					Logger::debug() << m_sLevelName << ": line " << pGridElement->Column() << " Gridpos is not valid: " << sGridPos << Logger::endl;
					bOk = false;
				}
			}
		}
		while(bOk && (pGridElement = pGridElement->NextSiblingElement()));
		
		if(bOk)
		{
			itlAddCube(cube);
		}
		else
		{
			Logger::debug() << m_sLevelName << ": Cube-loading NOT SUCCESSFUL" << Logger::endl;
		}

	}
	while(bOk && (pCubeElement = pCubeElement->NextSiblingElement()));

	return bOk;

}

bool Level::itlLoadGridFromXML(TiXmlElement* pGridElement, Grid& grid)
{
	bool bOk = true;

	std::string sGridPos;
	pGridElement->QueryStringAttribute("pos", &sGridPos);
	Logger::debug() << m_sLevelName << ": line " << pGridElement->Row() << ": Loading grid: " << sGridPos << Logger::endl;

	TiXmlElement* pDoorElement = pGridElement->FirstChildElement();

	do
	{
		assert(pDoorElement != NULL);

		std::string sTagName(pDoorElement->Value());
		if(sTagName.compare("door")!=0)
		{
			Logger::debug() << m_sLevelName << ": line " << pDoorElement->Row() << ": Tagname should be 'door', instead is: " << sTagName << Logger::endl;
			bOk = false;
		}
		else
		{
			int iX;
			int iY;

			pDoorElement->QueryIntAttribute("x", &iX);
			pDoorElement->QueryIntAttribute("y", &iY);

			bOk = grid.AddDoor(iX, iY);
			if(!bOk)
				Logger::debug() << m_sLevelName << ": line " << pDoorElement->Row() << ": x and y have to be an int between 1 and 9" << Logger::endl;
		}
	}
	while(bOk && (pDoorElement = pDoorElement->NextSiblingElement()));

	return bOk;
}

void Level::itlAddCube(Cube& cube)
{
	Logger::debug() << m_sLevelName << ": Add cube with id: " << cube.GetCubeID() << Logger::endl;
	m_Cubes.push_back(cube);
	m_iNumCubes++;
}