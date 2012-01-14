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


void Grid::SetPosition(std::string sPosition)
{
    m_sPosition = sPosition;
}

std::string Grid::GetPosition()
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

std::vector<glm::ivec2> Grid::CalcVertices()
{
	std::vector<glm::ivec2> doorscopy = m_vDoorPositions;
	std::vector<glm::ivec2> vertices;
	vertices.push_back(glm::ivec2(0,0));
	vertices.push_back(glm::ivec2(0,11));
	vertices.push_back(glm::ivec2(11,11));
	vertices.push_back(glm::ivec2(11,0));
	return ItlCalcVerticesRec(doorscopy, vertices);
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

std::vector<glm::ivec2> Grid::ItlCalcVerticesRec(std::vector<glm::ivec2> doors, std::vector<glm::ivec2> vertices)
{
	glm::ivec2 v3 = vertices.back(); vertices.pop_back();
	glm::ivec2 v2 = vertices.back(); vertices.pop_back();
	glm::ivec2 v1 = vertices.back(); vertices.pop_back();
	glm::ivec2 v0 = vertices.back(); vertices.pop_back();

	

	if(doors.size() == 0)
	{
		vertices.push_back(v0);
		vertices.push_back(v2);
		vertices.push_back(v1);
		vertices.push_back(v0);
		vertices.push_back(v3);
		vertices.push_back(v2);
		return vertices;
	}

	glm::ivec2 current_door = v0;
	int current_pos = -1;

	// get door that has lowest x and y and is inside the square defined of v0, v1, v2, v3
	for(int i = 0; i < doors.size(); i++)
	{
		glm::ivec2 temp = doors.at(i);
		if(temp.x >= v0.x && temp.y >= v0.y && temp.x < v2.x && temp.y < v2.y)
		{
			if(current_pos == -1 || temp.y < current_door.y || (temp.y == current_door.y && temp.x < current_door.x)) 
			{
				current_door = temp;
				current_pos = i;
			}
		}	
	}

	if(current_pos == -1)
	{
		vertices.push_back(v0);
		vertices.push_back(v2);
		vertices.push_back(v1);
		vertices.push_back(v0);
		vertices.push_back(v3);
		vertices.push_back(v2);
		return vertices;
	}

	//Logger::debug() << "current_door: " << current_door.x << "|" << current_door.y << Logger::endl;

	//create vector of the remaining doors
	std::vector<glm::ivec2> remaining_doors;
	for(int i = 0; i < doors.size(); i++)
	{
		if(current_pos != i)
			remaining_doors.push_back(doors.at(i));
	}

	//vertices of the door
	glm::ivec2 v4(current_door.x, current_door.y);
	glm::ivec2 v5(current_door.x, current_door.y+1);
	glm::ivec2 v6(current_door.x+1, current_door.y+1);
	glm::ivec2 v7(current_door.x+1, current_door.y);

	//other needed vertices
	glm::ivec2 v8(v0.x, current_door.y+1);
	glm::ivec2 v9(current_door.x, v0.y);
	glm::ivec2 v10(current_door.x+1, v0.y);
	glm::ivec2 v11(current_door.x+1, 11);

	//draw squares left of the door
	if(current_door.x > v0.x)
	{
		vertices.push_back(v0);
		vertices.push_back(v5);
		vertices.push_back(v8);

		vertices.push_back(v0);
		vertices.push_back(v9);
		vertices.push_back(v5);
	}

	if(current_door.y > v0.y)
	{
		vertices.push_back(v9);
		vertices.push_back(v7);
		vertices.push_back(v4);

		vertices.push_back(v9);
		vertices.push_back(v10);
		vertices.push_back(v7);
	}
	/*Logger::debug() << v0.x << "|" << v0.y << Logger::endl;
	Logger::debug() << v1.x << "|" << v1.y << Logger::endl;
	Logger::debug() << v2.x << "|" << v2.y << Logger::endl;
	Logger::debug() << v3.x << "|" << v3.y << Logger::endl;
	Logger::debug() << v4.x << "|" << v4.y << Logger::endl;
	Logger::debug() << v5.x << "|" << v5.y << Logger::endl;
	Logger::debug() << v6.x << "|" << v6.y << Logger::endl;
	Logger::debug() << v7.x << "|" << v7.y << Logger::endl;
	Logger::debug() << v8.x << "|" << v8.y << Logger::endl;
	Logger::debug() << v9.x << "|" << v9.y << Logger::endl;
	Logger::debug() << v10.x << "|" << v10.y << Logger::endl;
	Logger::debug() << v11.x << "|" << v11.y << Logger::endl;
	Logger::debug() << "##########" << Logger::endl;
	*/
	//subsquare1
	std::vector<glm::ivec2> vert_sub1;
	vert_sub1.push_back(v8);
	vert_sub1.push_back(v1);
	vert_sub1.push_back(v11);
	vert_sub1.push_back(v6);
	//subsquare1
	std::vector<glm::ivec2> vert_sub2;
	vert_sub2.push_back(v10);
	vert_sub2.push_back(v11);
	vert_sub2.push_back(v2);
	vert_sub2.push_back(v3);

	//calculate vertices of subsquare1
	if(v6.y < v1.y)
	{
		std::vector<glm::ivec2> vertices_subsquare1 = ItlCalcVerticesRec(remaining_doors, vert_sub1);
		//add subsquare1 vertices
		for(int i = 0; i < vertices_subsquare1.size(); i++)
		{
			vertices.push_back(vertices_subsquare1.at(i));
		}
	}

	//calculate vertices of subsquare2
	if(v6.x < v2.x)
	{
		std::vector<glm::ivec2> vertices_subsquare2 = ItlCalcVerticesRec(remaining_doors, vert_sub2);
		//add subsquare2 vertices
		for(int i = 0; i < vertices_subsquare2.size(); i++)
		{
			vertices.push_back(vertices_subsquare2.at(i));
		}
	}
	return vertices;
}
