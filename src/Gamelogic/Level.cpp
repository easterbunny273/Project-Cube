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
	m_sFileName = sFilename;

	Logger::debug() << "### Loading level from XML: " << m_sFileName << Logger::endl;

	TiXmlDocument document(m_sFileName);
	if(!document.LoadFile())
	{
		Logger::debug() << "XML-file " << m_sFileName << " could not be loaded! File does not exist or syntax error!" << Logger::endl;
		return false;
	}
	
	TiXmlHandle hHandle(&document);
	
	TiXmlElement *pRootElement = hHandle.FirstChildElement().Element();
	TiXmlElement *pElement;
	
	bool bOk = true;

	if(pRootElement == NULL)
	{
		Logger::debug() << m_sFileName << ": pRootElement is NULL" << Logger::endl;
		bOk = false;
	}

	if(bOk)
	{
		std::string sRootElementName(pRootElement->Value());
		
		assert (sRootElementName.length() > 0);

		if(sRootElementName.compare("level")!=0)
		{
			Logger::debug() << "XML-file " << m_sFileName << " does not contain a level!" << Logger::endl;
			return false;
		}
		
		TiXmlElement* pChildElement = pRootElement->FirstChildElement();
		
		do
		{
			std::string sTagName(pChildElement->Value());
			if(sTagName.compare("group")!=0)
			{
				Logger::debug() << m_sFileName << ": line " << pChildElement->Row() << ": Tagname should be a group, instead is: " << sTagName << Logger::endl;
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
		Logger::debug() << m_sFileName << ": COULD NOT LOAD LEVEL" << Logger::endl;
	else
		Logger::debug() << m_sFileName << ": LEVEL LOADING SUCCESSFUL" << Logger::endl;

	return bOk;
}

bool Level::WriteToXMLString(std::string &sFilename)
{
	bool bOk = true;

	TiXmlDocument doc;
	TiXmlElement* pElem;
	TiXmlElement* pGroup;

	//<level>
	TiXmlElement* pRootElement = new TiXmlElement("level");
	doc.LinkEndChild(pRootElement);

	//	<group name="attributes">
	pGroup = new TiXmlElement("group");
	pGroup->SetAttribute("name", "attributes");
	pRootElement->LinkEndChild(pGroup);
	
	//		<id>THE_ID</id>
	pElem = new TiXmlElement("id");
	stringstream s;
	std::string str;
	s << m_nLevelID;
	s >> str;
	pElem->LinkEndChild(new TiXmlText(str));
	pGroup->LinkEndChild(pElem);

	//		<name>THE_NAME</name>
	pElem = new TiXmlElement("name");
	pElem->LinkEndChild(new TiXmlText(m_sLevelName));
	pGroup->LinkEndChild(pElem);

	//	</group>
	//  <group name="cubes">
	pGroup = new TiXmlElement("group");
	pGroup->SetAttribute("name", "cubes");
	pRootElement->LinkEndChild(pGroup);

	//		<cube>
	Cube cube;
	Grid grid;
	glm::ivec2 door;
	std::vector<glm::ivec2> doors;
	TiXmlElement* pCube;
	TiXmlElement* pGrid;
	TiXmlElement* pDoor;
	for(int i = 0; i < m_Cubes.size(); i++)
	{
		cube = m_Cubes.at(i);
		pCube = new TiXmlElement("cube");
		pCube->SetAttribute("id", cube.GetCubeID());
		pCube->SetAttribute("x", cube.GetCubePosition().x);
		pCube->SetAttribute("y", cube.GetCubePosition().y);
		pCube->SetAttribute("z", cube.GetCubePosition().z);
		
		//		<grid>
		for(int j = 1; j <= 6; j++)
		{
			grid = cube.GetGrid(j);
			pGrid = new TiXmlElement("grid");
			pGrid->SetAttribute("pos", grid.getPosition());
			
			//		<door>
			doors = grid.GetDoorPositions();
			while(doors.size()>0)
			{
				door = doors.back();
				doors.pop_back();
				pDoor = new TiXmlElement("door");
				pDoor->SetAttribute("x", door.x);
				pDoor->SetAttribute("y", door.y);
				pGrid->LinkEndChild(pDoor);
			}
			pCube->LinkEndChild(pGrid);
		}
		pGroup->LinkEndChild(pCube);
	}



	doc.SaveFile(sFilename);
	return true;
}



/*#############################################################
*################## PRIVATE METHODS ##########################
#############################################################*/

bool Level::itlReadGroupFromXML(TiXmlElement *pGroup)
{
	std::string sName;
	pGroup->QueryStringAttribute("name", &sName);

	Logger::debug() << m_sFileName << ": line " << pGroup->Row() << ": Loading group: " << sName << Logger::endl;

	if(sName.compare("attributes")==0)
	{
		return itlLoadAttributesFromXML(pGroup);
	}
	else if(sName.compare("cubes")==0)
	{
		return itlLoadCubesFromXML(pGroup);
	}
	else
	{
		//TODO other group loading
	}
	return true;
	
}

bool Level::itlLoadAttributesFromXML(TiXmlElement *pAttribGroup)
{
	Logger::debug() << m_sFileName << ": Loading attributes" << Logger::endl;

	bool bOk = true;

	//TODO error handling
	TiXmlElement* pAttribElement = pAttribGroup->FirstChildElement("id");
	if(pAttribElement!=NULL)
	{
		const char* cText = pAttribElement->GetText();
		if(cText==NULL)
		{
			Logger::debug() << m_sFileName << ": line " << pAttribElement->Row() << ": Wrong declaration of the id of the level!" << Logger::endl;
			bOk = false;
		}
		else
		{
			stringstream strValue;
			strValue << cText;
			strValue >> m_nLevelID;
		}
		
	}
	else
	{
		Logger::debug() << m_sFileName << ": line " << pAttribElement->Row() << ": The level does not contain an id!" << Logger::endl;
		bOk = false;
	}

	pAttribElement = pAttribGroup->FirstChildElement("name");
	if(pAttribElement!=NULL)
	{
		const char* cText = pAttribElement->GetText();
		if(cText==NULL)
		{
			Logger::debug() << m_sFileName << ": line " << pAttribElement->Row() << ": Wrong declaration of the name of the level!" << Logger::endl;
			bOk = false;
		}
		else
		{
			m_sLevelName = cText;
		}
	}
	else
	{
		Logger::debug() << m_sFileName << ": line " << pAttribElement->Row() << ": The level does not contain a name!" << Logger::endl;
		bOk = false;
	}

	if(bOk)
		Logger::debug() << m_sFileName << ": Successful loaded attributes" << Logger::endl;

	return bOk;
}

bool Level::itlLoadCubesFromXML(TiXmlElement *pCubeGroup)
{
	Logger::debug() << m_sFileName << ": Loading cubegroup" << Logger::endl;

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
			Logger::debug() << m_sFileName << ": line " << pCubeElement->Row() << ": Tagname should be a cube, instead is: " << sTagName << Logger::endl;
			return false;
		}

		Logger::debug() << m_sFileName << ": line " << pCubeElement->Row() << ": Loading cube" << Logger::endl;

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
				Logger::debug() << m_sFileName << ": line " << pGridElement->Row() << ": Tagname should be 'grid', instead is: " << sTagName << Logger::endl;
				bOk = false;
			}
			else
			{
				std::string sGridPos;
				pGridElement->QueryStringAttribute("pos", &sGridPos);

				Grid grid;

				if(sGridPos.compare("Xplus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetXplus(grid);
				}
				else if(sGridPos.compare("Xminus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetXminus(grid);
				}
				else if(sGridPos.compare("Yplus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetYplus(grid);
				}
				else if(sGridPos.compare("Yminus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetYminus(grid);
				}
				else if(sGridPos.compare("Zplus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetZplus(grid);
				}
				else if(sGridPos.compare("Zminus")==0)
				{
					bOk = itlLoadGridFromXML(pGridElement, grid);
					cube.SetZminus(grid);
				}
				else
				{
					Logger::debug() << m_sFileName << ": line " << pGridElement->Column() << " Gridpos is not valid: " << sGridPos << Logger::endl;
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
			Logger::debug() << m_sFileName << ": Cube-loading NOT SUCCESSFUL" << Logger::endl;
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
	Logger::debug() << m_sFileName << ": line " << pGridElement->Row() << ": Loading grid: " << sGridPos << Logger::endl;

	TiXmlElement* pDoorElement = pGridElement->FirstChildElement();

	do
	{
		assert(pDoorElement != NULL);

		std::string sTagName(pDoorElement->Value());
		if(sTagName.compare("door")!=0)
		{
			Logger::debug() << m_sFileName << ": line " << pDoorElement->Row() << ": Tagname should be 'door', instead is: " << sTagName << Logger::endl;
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
				Logger::debug() << m_sFileName << ": line " << pDoorElement->Row() << ": x and y have to be an int between 1 and 9" << Logger::endl;
		}
	}
	while(bOk && (pDoorElement = pDoorElement->NextSiblingElement()));

	return bOk;
}

void Level::itlAddCube(Cube& cube)
{
	Logger::debug() << m_sFileName << ": Add cube with id: " << cube.GetCubeID() << Logger::endl;
	m_Cubes.push_back(cube);
	m_iNumCubes++;
}