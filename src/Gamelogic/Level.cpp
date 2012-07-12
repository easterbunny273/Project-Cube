#include "Gamelogic/Level.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"
#include "Gamelogic/Objects/Object.h"

#include <iostream>
using namespace std;


#include "PC_Logger.h"

Level::Level()
{
	m_iNumCubes = 0;
	itlInitSemanticSceneNode();
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

std::vector<Cube*> Level::GetCubes()
{
    return m_Cubes;
}

Cube* Level::GetCubeByPosition(int iX, int iY, int iZ)
{
    return GetCubeByPosition(glm::ivec3(iX, iY, iZ));
}

Cube* Level::GetCubeByPosition(glm::ivec3 iv3Position)
{
    Cube* cube;
    for(unsigned int i = 0; i < m_Cubes.size(); i++)
    {
        cube = m_Cubes.at(i);
        if(cube->GetCubePosition()==iv3Position)
        {
            return cube;
        }
    }
    return NULL;
}

bool Level::RotateX(const int iFactor)
{
    // rotation by 0
    if(iFactor%360 == 0)
            return true;
    // factor has to be a multiple of 90
    if(!(iFactor%90 == 0))
            return false;

    // rotation by 90
    if((iFactor-90)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldY = m_Cubes.at(i)->GetY();
            m_Cubes.at(i)->SetY(-m_Cubes.at(i)->GetZ());
            m_Cubes.at(i)->SetZ(iOldY);
            m_Cubes.at(i)->RotateX(iFactor);
        }
    }
    // rotation by 180
    else if((iFactor-180)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            m_Cubes.at(i)->SetY(-m_Cubes.at(i)->GetY());
            m_Cubes.at(i)->SetZ(-m_Cubes.at(i)->GetZ());
            m_Cubes.at(i)->RotateX(iFactor);
        }
    }
    // rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldY = m_Cubes.at(i)->GetY();
            m_Cubes.at(i)->SetY(m_Cubes.at(i)->GetZ());
            m_Cubes.at(i)->SetZ(-iOldY);
            m_Cubes.at(i)->RotateX(iFactor);
        }
    }
    return true;
}

bool Level::RotateY(const int iFactor)
{
    // rotation by 0
    if(iFactor%360 == 0)
            return true;
    // factor has to be a multiple of 90
    if(!(iFactor%90 == 0))
            return false;

    // rotation by 90
    if((iFactor-90)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldZ = m_Cubes.at(i)->GetZ();
            m_Cubes.at(i)->SetZ(-m_Cubes.at(i)->GetX());
            m_Cubes.at(i)->SetX(iOldZ);
            m_Cubes.at(i)->RotateY(iFactor);
        }
    }
    // rotation by 180
    else if((iFactor-180)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            m_Cubes.at(i)->SetX(-m_Cubes.at(i)->GetX());
            m_Cubes.at(i)->SetZ(-m_Cubes.at(i)->GetZ());
            m_Cubes.at(i)->RotateY(iFactor);
        }
    }
    // rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldZ = m_Cubes.at(i)->GetZ();
            m_Cubes.at(i)->SetZ(m_Cubes.at(i)->GetX());
            m_Cubes.at(i)->SetX(-iOldZ);
            m_Cubes.at(i)->RotateY(iFactor);
        }
    }
    return true;
}

bool Level::RotateZ(const int iFactor)
{
    // rotation by 0
    if(iFactor%360 == 0)
            return true;
    // factor has to be a multiple of 90
    if(!(iFactor%90 == 0))
            return false;

    // rotation by 90
    if((iFactor-90)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldX = m_Cubes.at(i)->GetX();
            m_Cubes.at(i)->SetX(-m_Cubes.at(i)->GetY());
            m_Cubes.at(i)->SetY(iOldX);
            m_Cubes.at(i)->RotateZ(iFactor);
        }
    }
    // rotation by 180
    else if((iFactor-180)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            m_Cubes.at(i)->SetX(-m_Cubes.at(i)->GetX());
            m_Cubes.at(i)->SetY(-m_Cubes.at(i)->GetY());
            m_Cubes.at(i)->RotateZ(iFactor);
        }
    }
    // rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        for(unsigned int i = 0; i < m_Cubes.size(); i++)
        {
            int iOldX = m_Cubes.at(i)->GetX();
            m_Cubes.at(i)->SetX(m_Cubes.at(i)->GetY());
            m_Cubes.at(i)->SetY(-iOldX);
            m_Cubes.at(i)->RotateZ(iFactor);
        }
    }
    return true;
}

bool Level::ReadFromXMLString(std::string sFile)
{
    //clear level
    Clear();

    //generate document by parsing the xml string
    TiXmlDocument document;
    document.Parse(sFile.c_str());

    Logger::debug() << "### Loading level from XML" << Logger::endl;

    //create handle
    TiXmlHandle hHandle(&document);
	
    //get root element
    TiXmlElement *pRootElement = hHandle.FirstChildElement().Element();

    assert(pRootElement!=NULL);
    std::string sRootElementName(pRootElement->Value());
    assert (sRootElementName.length() > 0);

    //root name has to be "level"
    if(sRootElementName.compare("level")!=0)
    {
        Logger::error() << "XML-file does not contain a level!" << Logger::endl;
        return false;
    }

    //get first child
    TiXmlElement* pChildElement = pRootElement->FirstChildElement();

    bool bOk = true;

    do //do-while to read every group in the file
    {
        std::string sTagName(pChildElement->Value());
        //tag of the child has to be "group"
        if(sTagName.compare("group")!=0)
        {
            Logger::error() << "Line " << pChildElement->Row() << ": Tagname should be \"group\", instead is: " << sTagName << Logger::endl;
            bOk = false;
        }
        else
        {
            // read group
            bOk = itlReadGroupFromXML(pChildElement);
        }
    }
    while(bOk && (pChildElement = pChildElement->NextSiblingElement()));

    if(bOk == false)
        Logger::error() << "COULD NOT LOAD LEVEL" << Logger::endl;
    else
        Logger::debug() << "LEVEL LOADING SUCCESSFUL" << Logger::endl;

    return bOk;
}

bool Level::WriteToXMLString(std::string &sString)
{
    std::ostringstream rssStream;

    rssStream << "<level>";
    rssStream <<    "<group name=\"attributes\">";
    rssStream <<        "<id>" << m_nLevelID << "</id>";
    rssStream <<        "<name>" << m_sLevelName << "</name>";
    rssStream <<    "</group>";
    rssStream <<    "<group name=\"cubes\">";

    Cube* cube;
    Grid grid;
    glm::ivec2 door;
    std::vector<glm::ivec2> doors;

    for(unsigned int i = 0; i < m_Cubes.size(); i++)
    {
        // write the cube and its attributes to the stream
        cube = m_Cubes.at(i);
        rssStream << "<cube ";
        rssStream << "id=\"" << cube->GetCubeID() << "\" ";
        rssStream << "x=\"" << cube->GetCubePosition().x << "\" ";
        rssStream << "y=\"" << cube->GetCubePosition().y << "\" ";
        rssStream << "z=\"" << cube->GetCubePosition().z << "\">";

        // write all grids of the cube and their attributes to the stream
        for(int j = 1; j <= 6; j++)
        {
            grid = cube->GetGrid(j);
            rssStream << "<grid pos=\"" << grid.GetPosition() << "\">";
			
            doors = grid.GetDoorPositions();
            // write all doors of a grid to the stream
            for(unsigned int k = 0; k < doors.size(); k++)
            {
                door = doors.at(k);
                rssStream << "<door x=\"" << door.x << "\" y=\"" << door.y << "\"/>";
            }
            rssStream << "</grid>";
        }
        rssStream << "</cube>";
    }
    rssStream << "</group>";

    // TODO add other groups

    rssStream << "</level>";

    sString = rssStream.str();

    return true;
}

bool Level::StoreLevelAsXMLFile(std::string sFilename)
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
        Logger::error() << "Saving level to file \"" << sFilename << "\" failed" << Logger::endl;

    return bOk;
}

bool Level::LoadLevelFromXMLFile(std::string sFilename)
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

        return ReadFromXMLString(sFileContent);
    }
    else
    {
        Logger::error() << "Level \"" << sFilename << "\" does not exist" << Logger::endl;
        return false;
    }
}

Object* Level::CreateObject(std::string sName, std::string sFileName)
{
	Logger::debug() << "C++: adding " << sName << " to the level"<< Logger::endl;
	Object* object = new Object(sName, sFileName);
	m_Objects.push_back(object);
	m_spSemanticScene->AddChild(object->GetSceneNode());
	return object;
}

LightObject* Level::CreateLight(std::string sName)
{
	Logger::debug() << "C++: adding " << sName << " to the level"<< Logger::endl;
	LightObject* light = new LightObject(sName);
	m_Lights.push_back(light);
	m_spSemanticScene->AddChild(light->GetSceneNode());
	return light;

}

Object* Level::GetObjectByName(std::string sName)
{
	for(unsigned int i = 0; i < m_Objects.size(); i++)
		if(sName == m_Objects.at(i)->GetName())
			return m_Objects.at(i);
	return NULL;
}

LightObject* Level::GetLightByName(std::string sName)
{
	for(unsigned int i = 0; i < m_Lights.size(); i++)
		if(sName == m_Lights.at(i)->GetName())
			return m_Lights.at(i);
	return NULL;
}

/*#############################################################
*################## PRIVATE METHODS ##########################
#############################################################*/

bool Level::itlReadGroupFromXML(TiXmlElement *pGroup)
{
    std::string sName;
    pGroup->QueryStringAttribute("name", &sName);

    Logger::debug() << "Line " << pGroup->Row() << ": Loading group: " << sName << Logger::endl;

    // read level attributes
    if(sName.compare("attributes")==0)
    {
        return itlLoadAttributesFromXML(pGroup);
    }
    // read cubes
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
    Logger::debug() << "Loading attributes" << Logger::endl;

    bool bOk = true;

    //get the level-id
    TiXmlElement* pAttribElement = pAttribGroup->FirstChildElement("id");
    if(pAttribElement!=NULL)
    {
        const char* cText = pAttribElement->GetText();
        if(cText==NULL)
        {
            Logger::error() << "Line " << pAttribElement->Row() << ": Wrong declaration of the id of the level!" << Logger::endl;
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
        Logger::error() << "Line " << pAttribElement->Row() << ": The level does not contain an id!" << Logger::endl;
        bOk = false;
    }

    // get the level-name
    pAttribElement = pAttribGroup->FirstChildElement("name");
    if(pAttribElement!=NULL)
    {
        const char* cText = pAttribElement->GetText();
        if(cText==NULL)
        {
            Logger::error() << "Line " << pAttribElement->Row() << ": Wrong declaration of the name of the level!" << Logger::endl;
            bOk = false;
        }
        else
        {
            m_sLevelName = cText;
        }
    }
    else
    {
        Logger::error() << "Line " << pAttribElement->Row() << ": The level does not contain a name!" << Logger::endl;
        bOk = false;
    }

    //if all attributes were loaded successfully
    if(bOk)
        Logger::debug() << "Successful loaded attributes" << Logger::endl;

    return bOk;
}

bool Level::itlLoadCubesFromXML(TiXmlElement *pCubeGroup)
{
    Logger::debug() << "Loading cubegroup" << Logger::endl;

    bool bOk = true;

    //get first cube from the cube group
    TiXmlElement* pCubeElement = pCubeGroup->FirstChildElement();

    do
    {
        assert (pCubeElement != NULL);

        // tagname has to be "cube
        std::string sTagName(pCubeElement->Value());
        if(sTagName.compare("cube")!=0)
        {
            Logger::error() << "Line " << pCubeElement->Row() << ": Tagname should be \"cube\", instead is: " << sTagName << Logger::endl;
            return false;
        }

        Logger::debug() << "Line " << pCubeElement->Row() << ": Loading cube" << Logger::endl;

        unsigned int iCubeID;
        int iX;
        int iY;
        int iZ;

        // get the cube attributes
        pCubeElement->QueryUnsignedAttribute("id", &iCubeID);
        pCubeElement->QueryIntAttribute("x", &iX);
        pCubeElement->QueryIntAttribute("y", &iY);
        pCubeElement->QueryIntAttribute("z", &iZ);

        Cube* cube = new Cube(iCubeID, iX, iY, iZ);

        //get first grid of the cube
        TiXmlElement* pGridElement = pCubeElement->FirstChildElement();

        if(pGridElement!=NULL)
        {
            //get all the grids!
            do
            {
                //tagname has to be grid
                std::string sTagName(pGridElement->Value());
                if(sTagName.compare("grid")!=0)
                {
                    Logger::error() << "Line " << pGridElement->Row() << ": Tagname should be \"grid\", instead is: " << sTagName << Logger::endl;
                    bOk = false;
                }
                else
                {
                    //insert grid at the right position

                    std::string sGridPos;
                    pGridElement->QueryStringAttribute("pos", &sGridPos);

                    Grid grid;

                    if(sGridPos.compare("Xplus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetXplus(grid);
                    }
                    else if(sGridPos.compare("Xminus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetXminus(grid);
                    }
                    else if(sGridPos.compare("Yplus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetYplus(grid);
                    }
                    else if(sGridPos.compare("Yminus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetYminus(grid);
                    }
                    else if(sGridPos.compare("Zplus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetZplus(grid);
                    }
                    else if(sGridPos.compare("Zminus")==0)
                    {
                        bOk = itlLoadGridFromXML(pGridElement, grid);
                        cube->SetZminus(grid);
                    }
                    else
                    {
                        Logger::error() << "Line " << pGridElement->Column() << " Gridpos is not valid: " << sGridPos << Logger::endl;
                        bOk = false;
                    }
                }
            }
            while(bOk && (pGridElement = pGridElement->NextSiblingElement()));
        }

        if(bOk)
        {
            //add cube to the level
            itlAddCube(cube);
        }
        else
        {
            Logger::error() << "Cube-loading NOT SUCCESSFUL" << Logger::endl;
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
    Logger::debug() << "Line " << pGridElement->Row() << ": Loading grid: " << sGridPos << Logger::endl;

    //get first door
    TiXmlElement* pDoorElement = pGridElement->FirstChildElement();

    do // get all the doors!
    {
        assert(pDoorElement != NULL);

        //tagname has to be "door"
        std::string sTagName(pDoorElement->Value());
        if(sTagName.compare("door")!=0)
        {
            Logger::error() << "Line " << pDoorElement->Row() << ": Tagname should be \"door\", instead is: " << sTagName << Logger::endl;
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
                Logger::error() << "Line " << pDoorElement->Row() << ": x and y have to be an int between 1 and 9" << Logger::endl;
        }
    }
    while(bOk && (pDoorElement = pDoorElement->NextSiblingElement()));

    return bOk;
}

void Level::itlAddCube(Cube* cube)
{
    Logger::debug() << "Add cube with id: " << cube->GetCubeID() << Logger::endl;
    m_Cubes.push_back(cube);
    m_iNumCubes++;

	// Create semantic node of the cube and add it to the root semantic node
	std::shared_ptr<ISemanticSceneNode> spCubeSceneNode = Cube_SemSceneNode::Create(cube);

	m_spSemanticScene->AddChild(spCubeSceneNode);

}


void Level::itlInitSemanticSceneNode()
{
	m_spCamera = Bamboo::PerspectiveCamera::Create(45.0f, 1.33f, 0.01f, 100.0f, glm::vec3(-0.2f, 0.2f, 0.0f), 90.0f, -50.0f);	
	m_spSemanticScene = Camera_SemSceneNode::Create(m_spCamera);
}
