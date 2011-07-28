#include "Grid.h"
#include "Logger.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

Grid::Grid(Grid& grid)
{
	for(int i = 0; i < grid.GetDoorPositions().size(); i++)
	{
		this->AddDoor(grid.GetDoorPositions().at(i));
	}
}

bool Grid::AddDoor(glm::vec2 doorPosition)
{
	bool bInsert = CheckDoorInsertion(doorPosition.x, doorPosition.y);

	if(bInsert)
		m_vDoorPositions.push_back(doorPosition);
	
	return bInsert;
}

bool Grid::AddDoor(int iDoorX, int iDoorY)
{
	bool bInsert = CheckDoorInsertion(iDoorX, iDoorY);

	if(bInsert)
		m_vDoorPositions.push_back(glm::vec2(iDoorX, iDoorY));
	
	return bInsert;
}

bool Grid::CheckDoorInsertion(const int iX, const int iY)
{
	if(iX < 1 || iX > 9 || iY < 1 || iY > 9)
		return false;

	for(int i = 0; i < m_vDoorPositions.size(); i++)
	{
		glm::vec2 current = m_vDoorPositions.at(i);
		if(iX < current.x-1 || iX > current.x+1 || iY < current.y-1 || iY > current.y+1)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}


void Grid::ClearGrid()
{
	m_vDoorPositions.clear();
}

std::vector<glm::vec2> Grid::GetDoorPositions()
{
	return m_vDoorPositions;
}

bool Grid::RotateGrid(const int iFactor)
{
	if(iFactor == 0)
		return true;
	if(iFactor < -3 || iFactor > 3)
		return false;

	std::vector<glm::vec2> temp = m_vDoorPositions;
	m_vDoorPositions.clear();

	if(iFactor == 1 || iFactor == -3)
	{
		for(int i = 0; i < temp.size(); i++)
		{
			glm::vec2 old = temp.at(i);
			m_vDoorPositions.push_back(glm::vec2(10-old.y, old.x));
		}
	}
	else if(iFactor == 2 || iFactor == -2)
	{
		for(int i = 0; i < temp.size(); i++)
		{
			glm::vec2 old = temp.at(i);
			m_vDoorPositions.push_back(glm::vec2(10-old.x, 10-old.y));
		}
	}
	else if(iFactor == 3 || iFactor == -1)
	{
		for(int i = 0; i < temp.size(); i++)
		{
			glm::vec2 old = temp.at(i);
			m_vDoorPositions.push_back(glm::vec2(old.y, 10-old.x));
		}
	}
	return true;
}

void Grid::MirrorGridVertical()
{
	std::vector<glm::vec2> temp = m_vDoorPositions;
	m_vDoorPositions.clear();
	for(int i = 0; i < temp.size(); i++)
	{
		m_vDoorPositions.push_back(glm::vec2(10-temp.at(i).x, temp.at(i).y));
	}
}

void Grid::MirrorGridHorizontal()
{
	std::vector<glm::vec2> temp = m_vDoorPositions;
	m_vDoorPositions.clear();
	for(int i = 0; i < temp.size(); i++)
	{
		m_vDoorPositions.push_back(glm::vec2(temp.at(i).x, 10-temp.at(i).y));
	}
}