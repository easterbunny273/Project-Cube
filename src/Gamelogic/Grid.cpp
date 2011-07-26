#include "Grid.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

bool Grid::AddDoor(glm::vec2 doorPosition)
{
	//TODO: check if new door position is valid

	m_vDoorPositions.push_back(doorPosition);
	return true;
}

bool Grid::AddDoor(int iDoorX, int iDoorY)
{
	//TODO: check if new door position is valid

	m_vDoorPositions.push_back(glm::vec2(iDoorX, iDoorY));
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
		while(!temp.empty())
		{
			glm::vec2 old = temp.back();
			temp.pop_back();
			m_vDoorPositions.push_back(glm::vec2(10-old.y, old.x));
		}
	}
	else if(iFactor == 2 || iFactor == -2)
	{
		while(!temp.empty())
		{
			glm::vec2 old = temp.back();
			temp.pop_back();
			m_vDoorPositions.push_back(glm::vec2(10-old.x, 10-old.y));
		}
	}
	else if(iFactor == 3 || iFactor == -1)
	{
		while(!temp.empty())
		{
			glm::vec2 old = temp.back();
			temp.pop_back();
			m_vDoorPositions.push_back(glm::vec2(old.y, 10-old.x));
		}
	}
	return true;
}