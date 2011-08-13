#include "Gamelogic/Cube.h"
#include "Logger.h"

Cube::Cube()
{
	m_nCubeID = -1;
}

Cube::Cube(int iCubeID, int iX, int iY, int iZ)
{
	m_nCubeID = iCubeID;
	m_CubePosition = glm::ivec3(iX, iY, iZ);
}

Cube::~Cube()
{
}

unsigned int Cube::GetCubeID()
{
	return m_nCubeID;
}

glm::ivec3 Cube::GetCubePosition()
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
	if(iFactor == 0)
		return true;
	if(!(iFactor%90 == 0))
		return false;

	if((iFactor-90)%360 == 0)
	{
		m_Xplus.RotateGrid(-90);
		m_Xminus.RotateGrid(-90);
		m_Zplus.MirrorGridHorizontal();
		m_Zminus.MirrorGridHorizontal();
		itlSwitchGrids(m_Zplus, m_Yplus, m_Zminus, m_Yminus);
	}
	else if((iFactor-180)%360 == 0)
	{
		m_Xplus.RotateGrid(-180);
		m_Xminus.RotateGrid(-180);
		m_Yplus.MirrorGridHorizontal();
		m_Yminus.MirrorGridHorizontal();
		m_Zplus.MirrorGridHorizontal();
		m_Zminus.MirrorGridHorizontal();
		itlSwitchGrids(m_Yplus, m_Yminus);
		itlSwitchGrids(m_Zplus, m_Zminus);
	}
	else if((iFactor-270)%360 == 0)
	{
		m_Xplus.RotateGrid(-270);
		m_Xminus.RotateGrid(-270);
		m_Yplus.MirrorGridHorizontal();
		m_Yminus.MirrorGridHorizontal();
		itlSwitchGrids(m_Zplus, m_Yminus, m_Zminus, m_Yplus);
	}
	return true;
}

bool Cube::RotateY(const int iFactor)
{
	if(iFactor == 0)
		return true;
	if(!(iFactor%90 == 0))
		return false;

	if((iFactor-90)%360 == 0)
	{
		m_Yplus.RotateGrid(-90);
		m_Yminus.RotateGrid(-90);
		m_Xplus.MirrorGridVertical();
		m_Xminus.MirrorGridVertical();
		itlSwitchGrids(m_Xminus, m_Zminus, m_Xplus, m_Zplus);
	}
	else if((iFactor-180)%360 == 0)
	{
		m_Yplus.RotateGrid(-180);
		m_Yminus.RotateGrid(-180);
		m_Xplus.MirrorGridVertical();
		m_Xminus.MirrorGridVertical();
		m_Zplus.MirrorGridVertical();
		m_Zminus.MirrorGridVertical();
		itlSwitchGrids(m_Xplus, m_Xminus);
		itlSwitchGrids(m_Zplus, m_Zminus);
	}
	else if((iFactor-270)%360 == 0)
	{
		m_Yplus.RotateGrid(-270);
		m_Yminus.RotateGrid(-270);
		m_Zplus.MirrorGridVertical();
		m_Zminus.MirrorGridVertical();
		itlSwitchGrids(m_Xplus, m_Zminus, m_Xminus, m_Zplus);
	}
	return true;
}

bool Cube::RotateZ(const int iFactor)
{
	if(iFactor == 0)
		return true;
	if(!(iFactor%90 == 0))
		return false;

	if((iFactor-90)%360 == 0)
	{
		m_Zplus.RotateGrid(-90);
		m_Zminus.RotateGrid(-90);
		m_Xplus.RotateGrid(-90);
		m_Xminus.RotateGrid(-90);
		m_Yplus.RotateGrid(-90);
		m_Yplus.MirrorGridVertical();
		m_Yminus.RotateGrid(-90);
		m_Yminus.MirrorGridVertical();
		itlSwitchGrids(m_Xplus, m_Yminus, m_Xminus, m_Yplus);
	}
	else if((iFactor-180)%360 == 0)
	{
		m_Zplus.RotateGrid(-180);
		m_Zminus.RotateGrid(-180);
		m_Xplus.MirrorGridHorizontal();
		m_Xminus.MirrorGridHorizontal();
		m_Yplus.MirrorGridVertical();
		m_Yminus.MirrorGridVertical();
		itlSwitchGrids(m_Xplus, m_Xminus);
		itlSwitchGrids(m_Yplus, m_Yminus);
	}
	else if((iFactor-270)%360 == 0)
	{
		m_Zplus.RotateGrid(-270);
		m_Zminus.RotateGrid(-270);
		m_Yplus.RotateGrid(90);
		m_Yminus.RotateGrid(90);
		m_Xplus.RotateGrid(-90);
		m_Xplus.MirrorGridVertical();
		m_Xminus.RotateGrid(-90);
		m_Xminus.MirrorGridVertical();
		m_Zminus.MirrorGridVertical();
		itlSwitchGrids(m_Xplus, m_Yplus, m_Xminus, m_Yminus);
	}
	return true;
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

void Cube::itlSwitchGrids(Grid& grid1, Grid& grid2)
{
	Grid temp = grid1;
	grid1 = grid2;
	grid2 = temp;
}
