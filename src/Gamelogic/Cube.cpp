#include "Gamelogic/Cube.h"
#include "PC_Logger.h"

Cube::Cube()
{
}

Cube::Cube(unsigned int iCubeID, int iX, int iY, int iZ)
{
	m_nCubeID = iCubeID;
	m_CubePosition = glm::ivec3(iX, iY, iZ);
	itlUpdateGridPositions();
}

Cube::~Cube()
{
}

/*#############################################################
*################## PUBLIC METHODS ###########################
#############################################################*/

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

        // update the grid positions at each grid
	itlUpdateGridPositions();
}

void Cube::SetXplus(Grid& xplus)
{
	m_Xplus = xplus;
	m_Xplus.SetPosition("Xplus");
}

void Cube::SetXminus(Grid& xminus)
{
	m_Xminus = xminus;
	m_Xminus.SetPosition("Xminus");
}

void Cube::SetYplus(Grid& yplus)
{
	m_Yplus = yplus;
	m_Yplus.SetPosition("Yplus");
}

void Cube::SetYminus(Grid& yminus)
{
	m_Yminus = yminus;
	m_Yminus.SetPosition("Yminus");
}

void Cube::SetZplus(Grid& zplus)
{
	m_Zplus = zplus;
	m_Zplus.SetPosition("Zplus");
}

void Cube::SetZminus(Grid& zminus)
{
	m_Zminus = zminus;
	m_Zminus.SetPosition("Zminus");
}

int Cube::GetX()
{
    return m_CubePosition.x;
}

int Cube::GetY()
{
    return m_CubePosition.y;
}

int Cube::GetZ()
{
    return m_CubePosition.z;
}

void Cube::SetX(int iX)
{
    m_CubePosition.x = iX;
}

void Cube::SetY(int iY)
{
    m_CubePosition.y = iY;
}

void Cube::SetZ(int iZ)
{
    m_CubePosition.z = iZ;
}

bool Cube::RotateX(const int iFactor)
{
    //rotation by 0
    if(iFactor%360 == 0)
        return true;
    //a cube can only be rotated by a multiple of 90
    if(!(iFactor%90 == 0))
        return false;

    // rotation by 90
    if((iFactor-90)%360 == 0)
    {
        m_Xplus.RotateGrid(-90);
        m_Xminus.RotateGrid(-90);
        m_Zplus.MirrorGridHorizontal();
        m_Zminus.MirrorGridHorizontal();
        itlSwitchGrids(m_Zplus, m_Yplus, m_Zminus, m_Yminus);
    }
    //rotation by 180
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
    //rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        m_Xplus.RotateGrid(-270);
        m_Xminus.RotateGrid(-270);
        m_Yplus.MirrorGridHorizontal();
        m_Yminus.MirrorGridHorizontal();
        itlSwitchGrids(m_Zplus, m_Yminus, m_Zminus, m_Yplus);
    }
    // update the position string inside each grid
    itlUpdateGridPositions();
    return true;
}

bool Cube::RotateY(const int iFactor)
{
    //rotation by 0
    if(iFactor%360 == 0)
        return true;
    //a cube can only be rotated by a multiple of 90
    if(!(iFactor%90 == 0))
        return false;

    //rotation by 90
    if((iFactor-90)%360 == 0)
    {
        m_Yplus.RotateGrid(-90);
        m_Yminus.RotateGrid(-90);
        m_Xplus.MirrorGridVertical();
        m_Xminus.MirrorGridVertical();
        itlSwitchGrids(m_Xminus, m_Zminus, m_Xplus, m_Zplus);
    }
    //rotation by 180
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
    //rotation by 270
    else if((iFactor-270)%360 == 0)
    {
        m_Yplus.RotateGrid(-270);
        m_Yminus.RotateGrid(-270);
        m_Zplus.MirrorGridVertical();
        m_Zminus.MirrorGridVertical();
        itlSwitchGrids(m_Xplus, m_Zminus, m_Xminus, m_Zplus);
    }
    // update the position string inside each grid
    itlUpdateGridPositions();
    return true;
}

bool Cube::RotateZ(const int iFactor)
{
    //rotation by 0
    if(iFactor%360 == 0)
        return true;
    //a cube can only be rotated by a multiple of 90
    if(!(iFactor%90 == 0))
        return false;

    // rotation by 90
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
    // rotation by 180
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
    // rotation by 270
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
    // update the position string inside each grid
    itlUpdateGridPositions();
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
        default: return Grid();
    }
}

std::vector<glm::vec3> Cube::BuildCubeVertices()
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec2> xPlusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> xMinusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> yPlusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> yMinusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> zPlusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> zMinusVertices = m_Xplus.CalcVertices();

	for(int i = 0; i < xPlusVertices.size(); i++)
		vertices.push_back(glm::vec3(11, xPlusVertices.at(i).y, xPlusVertices.at(i).x));
	for(int i = 0; i < xMinusVertices.size(); i++)
		vertices.push_back(glm::vec3(0, xMinusVertices.at(i).y, xMinusVertices.at(i).x));
	for(int i = 0; i < yPlusVertices.size(); i++)
		vertices.push_back(glm::vec3(yPlusVertices.at(i).x, 11, yPlusVertices.at(i).y));
	for(int i = 0; i < yMinusVertices.size(); i++)
		vertices.push_back(glm::vec3(yMinusVertices.at(i).x, 0, yMinusVertices.at(i).y));
	for(int i = 0; i < zPlusVertices.size(); i++)
		vertices.push_back(glm::vec3(zPlusVertices.at(i).x, zPlusVertices.at(i).y, 11));
	for(int i = 0; i < zMinusVertices.size(); i++)
		vertices.push_back(glm::vec3(zMinusVertices.at(i).x, zMinusVertices.at(i).y, 0));
	return vertices;
}


/*#############################################################
*################## PRIVATE METHODS ##########################
#############################################################*/

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

void Cube::itlUpdateGridPositions()
{
    m_Xplus.SetPosition("Xplus");
    m_Xminus.SetPosition("Xminus");
    m_Yplus.SetPosition("Yplus");
    m_Yminus.SetPosition("Yminus");
    m_Zplus.SetPosition("Zplus");
    m_Zminus.SetPosition("Zminus");
}
