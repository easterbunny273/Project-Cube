#include "Gamelogic/Grid.h"
#include "PC_Logger.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

/*#############################################################
*################## PUBLIC METHODS ###########################
#############################################################*/

bool Grid::AddDoor(glm::ivec2 doorPosition)
{
    // check if the door can be inserted
    bool bInsert = ItlCheckDoorInsertion(doorPosition.x, doorPosition.y);

    // insert the door if possible
    if(bInsert)
        m_vDoorPositions.push_back(doorPosition);
	
    return bInsert;
}

bool Grid::AddDoor(int iDoorX, int iDoorY)
{
    // check if the door can be inserted
    bool bInsert = ItlCheckDoorInsertion(iDoorX, iDoorY);

    // insert the door if possible
    if(bInsert)
        m_vDoorPositions.push_back(glm::ivec2(iDoorX, iDoorY));
	
    return bInsert;
}

void Grid::ClearGrid()
{
    // deletes all doors
    m_vDoorPositions.clear();
}

std::vector<glm::ivec2> Grid::GetDoorPositions()
{
    return m_vDoorPositions;
}

bool Grid::RotateGrid(const int iFactor)
{
    // rotation by 0
    if(iFactor%360 == 0)
        return true;
    // factor has to be a multiple of 90
    if(!(iFactor%90 == 0))
        return false;

    glm::ivec2 old;

    // rotation by 90
    if((iFactor-90)%360 == 0)
    {
        for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
        {
            old = m_vDoorPositions.at(i);
            m_vDoorPositions.at(i) = glm::ivec2(10-old.y, old.x);
        }
    }
    // rotation by 180
    else if((iFactor-180)%360 == 0)
    {
        for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
        {
            old = m_vDoorPositions.at(i);
            m_vDoorPositions.at(i) = glm::ivec2(10-old.x, 10-old.y);
        }
    }
    // rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
        {
            old = m_vDoorPositions.at(i);
            m_vDoorPositions.at(i) = glm::ivec2(old.y, 10-old.x);
        }
    }
    return true;
}

void Grid::MirrorGridVertical()
{
    glm::ivec2 old;
    for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
    {
        old = m_vDoorPositions.at(i);
        m_vDoorPositions.at(i) = glm::ivec2(10-old.x, old.y);
    }
}

void Grid::MirrorGridHorizontal()
{
    glm::ivec2 old;
    for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
    {
        old = m_vDoorPositions.at(i);
        m_vDoorPositions.at(i) = glm::ivec2(old.x, 10-old.y);
    }
}


void Grid::setPosition(std::string sPosition)
{
    m_sPosition = sPosition;
}

std::string Grid::getPosition()
{
    return m_sPosition;
}


void Grid::PrintGrid()
{
    for(int i = 0; i < m_vDoorPositions.size(); i++)
    {
        Logger::debug() << "grid: " << m_sPosition << "; door: (" << m_vDoorPositions.at(i).x << "," << m_vDoorPositions.at(i).y << ")" << Logger::endl;
    }
}


/*#############################################################
*################## PRIVATE METHODS ##########################
#############################################################*/

bool Grid::ItlCheckDoorInsertion(const int iX, const int iY)
{
    // values have to be between 1 and 9
    if(iX < 1 || iX > 9 || iY < 1 || iY > 9)
        return false;

    //current door that the values are compared with
    glm::ivec2 current;
    for(unsigned int i = 0; i < m_vDoorPositions.size(); i++)
    {
        current = m_vDoorPositions.at(i);
        // the door to be inserted has to have at least a gap of 1 at x and y
        if(iX < current.x-1 || iX > current.x+1 || iY < current.y-1 || iY > current.y+1)
        {
            continue;
        }
        else
        {
            // door can not be inserted
            return false;
        }
    }
    // door can be inserted
    return true;
}
