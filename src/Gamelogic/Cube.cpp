#include "Cube.h"
#include "Logger.h"

Cube::Cube()
{
}

Cube::~Cube()
{
}

unsigned int Cube::GetCubeID()
{
	return m_nCubeID;
}

glm::vec3 Cube::GetCubePosition()
{
	return m_CubePosition;
}

glm::mat4 Cube::GetTransformation()
{
	return m_CubeTransformation;
}

void Cube::SetGrids(Grid& xplus, Grid& xminus, Grid& yplus, Grid& yminus, Grid& zplus, Grid& zminus)
{
	m_Xplus = xplus;
	m_Xminus = xminus;
	m_Yplus = yplus;
	m_Yminus = yminus;
	m_Zplus = zplus;
	m_Zminus = zminus;
}

bool Cube::RotateX(const int iFactor)
{
	return itlRotateGrids(m_Xplus, m_Xminus, m_Zplus, m_Zminus, m_Yplus, m_Yminus, iFactor);
}

bool Cube::RotateY(const int iFactor)
{
	return false;
}

bool Cube::RotateZ(const int iFactor)
{
	return false;
}

Grid Cube::GetGrid(const unsigned int nID)
{
	switch(nID)
	{
	case 1: return m_Xplus;
	case 2: return m_Xminus;
	case 3: return m_Yplus;
	case 4: return m_Yminus;
	case 5: return m_Zplus;
	case 6: return m_Zminus;
	}
}

void Cube::itlSwitchGrids(Grid& grid1, Grid& grid2, Grid& grid3, Grid& grid4)
{
	Grid temp = grid1;
	grid1 = grid2;
	grid2 = grid3;
	grid3 = grid4;
	grid4 = temp;
}

bool Cube::itlRotateGrids(Grid& axisGridPlus, Grid& axisGridMinus, Grid& gridLeft, Grid& gridRight, Grid& gridTop, Grid& gridBot, const int iFactor)
{
	if(iFactor == 0)
		return true;
	if(iFactor < -3 || iFactor > 3)
		return false;

	if(iFactor == 1 || iFactor == -3)
	{
		axisGridPlus.RotateGrid(-1);
		axisGridMinus.RotateGrid(-1);
		gridLeft.MirrorGridHorizontal();
		gridRight.MirrorGridHorizontal();
		itlSwitchGrids(gridLeft, gridTop, gridRight, gridBot);
	}
	else if(iFactor == 2 || iFactor == -2)
	{
	}
	else if(iFactor == 3 || iFactor == -1)
	{
	}
	return true;
}