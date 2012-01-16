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

void Cube::BuildCubeVertices()
{
	std::vector<glm::ivec2> xPlusVertices = m_Xplus.CalcVertices();
	std::vector<glm::ivec2> xMinusVertices = m_Xminus.CalcVertices();
	std::vector<glm::ivec2> yPlusVertices = m_Yplus.CalcVertices();
	std::vector<glm::ivec2> yMinusVertices = m_Yminus.CalcVertices();
	std::vector<glm::ivec2> zPlusVertices = m_Zplus.CalcVertices();
	std::vector<glm::ivec2> zMinusVertices = m_Zminus.CalcVertices();

	for(unsigned int i = 0; i < xPlusVertices.size(); i++)
	{
		m_CubeVertices.push_back(glm::vec3(11, xPlusVertices.at(i).y, xPlusVertices.at(i).x));
		m_CubeNormals.push_back(glm::vec3(-1, 0, 0));
		m_CubeTexCoords.push_back(glm::vec2(xPlusVertices.at(i).x / 11.0f, xPlusVertices.at(i).y / 11.0f));
	}
	while(xMinusVertices.size() != 0)
	{
		m_CubeVertices.push_back(glm::vec3(0, xMinusVertices.back().y, xMinusVertices.back().x));
		m_CubeNormals.push_back(glm::vec3(1, 0, 0));
		m_CubeTexCoords.push_back(glm::vec2(xMinusVertices.back().x / 11.0f, xMinusVertices.back().y / 11.0f));
		xMinusVertices.pop_back();
	}
	for(unsigned int i = 0; i < yPlusVertices.size(); i++)
	{
		m_CubeVertices.push_back(glm::vec3(yPlusVertices.at(i).x, 11, yPlusVertices.at(i).y));
		m_CubeNormals.push_back(glm::vec3(0, -1, 0));
		m_CubeTexCoords.push_back(glm::vec2(yPlusVertices.at(i).x / 11.0f, yPlusVertices.at(i).y / 11.0f));
	}
	while(yMinusVertices.size() != 0)
	{
		m_CubeVertices.push_back(glm::vec3(yMinusVertices.back().x, 0, yMinusVertices.back().y));
		m_CubeNormals.push_back(glm::vec3(0, 1, 0));
		m_CubeTexCoords.push_back(glm::vec2(yMinusVertices.back().x / 11.0f, yMinusVertices.back().y / 11.0f));
		yMinusVertices.pop_back();
	}
	while(zPlusVertices.size() != 0)
	{
		m_CubeVertices.push_back(glm::vec3(zPlusVertices.back().x, zPlusVertices.back().y, 11));
		m_CubeNormals.push_back(glm::vec3(0, 0, -1));
		m_CubeTexCoords.push_back(glm::vec2(zPlusVertices.back().x / 11.0f, zPlusVertices.back().y / 11.0f));
		zPlusVertices.pop_back();
	}
	for(unsigned int i = 0; i < zMinusVertices.size(); i++)
	{
		m_CubeVertices.push_back(glm::vec3(zMinusVertices.at(i).x, zMinusVertices.at(i).y, 0));
		m_CubeNormals.push_back(glm::vec3(0, 0, 1));
		m_CubeTexCoords.push_back(glm::vec2(zMinusVertices.at(i).x / 11.0f, zMinusVertices.at(i).y / 11.0f));
	}
}

std::vector<glm::vec3> Cube::GetVertices()
{
	return m_CubeVertices;
}

std::vector<glm::vec3> Cube::GetNormals()
{
	return m_CubeNormals;
}

std::vector<glm::vec2> Cube::GetTexCoords()
{
	return m_CubeTexCoords;
}


std::shared_ptr<GeometryData::GenericObject> Cube::GenerateGenericObject()
{
  std::shared_ptr<GeometryData::GenericObject> spNewObject(new GeometryData::GenericObject(1));

  std::shared_ptr<GeometryData::GenericMesh> spMesh = spNewObject->GetMesh(0).lock();

  GeometryData::GenericMesh *pMesh = spMesh.get();

  // generate vertices
  BuildCubeVertices();

  std::vector<glm::vec3> vVertices = GetVertices();
  std::vector<glm::vec3> vNormals = GetNormals();
  std::vector<glm::vec2> vTexCoords = GetTexCoords();

  assert (vVertices.size() == vNormals.size());
  assert (vVertices.size() == vTexCoords.size());

  float *pfVertices = new float[vVertices.size() * 3];
  float *pfNormals = new float[vNormals.size() * 3];
  float *pfTexCoords = new float[vTexCoords.size() * 2];

  unsigned int *pnIndices = new unsigned int [vVertices.size()];

  for (int i=0; i < vVertices.size(); i++)
    {
      pfVertices[3*i] = vVertices[i].x / 11.0 - 0.5;
      pfVertices[3*i + 1] = vVertices[i].y / 11.0 - 0.5;
      pfVertices[3*i + 2] = vVertices[i].z / 11.0 - 0.5;

      pfNormals[3*i + 0] = vNormals[i].x;
      pfNormals[3*i + 1] = vNormals[i].y;
      pfNormals[3*i + 2] = vNormals[i].z;

      pfTexCoords[2*i + 0] = vTexCoords[i].x * 10;
      pfTexCoords[2*i + 1] = vTexCoords[i].y * 10;

      pnIndices[i] = i;
    }

  pMesh->AddAttributeValues(GeometryData::GenericData::DATA_VERTICES, vVertices.size() * 3, pfVertices);
  pMesh->AddAttributeValues(GeometryData::GenericData::DATA_NORMALS, vNormals.size() * 3, pfNormals);
  //pMesh->AddAttributeValues(GeometryData::GenericData::DATA_TEXCOORDS, vTexCoords.size() * 2, pfTexCoords);

  pMesh->AddIndices(vVertices.size(), pnIndices);
  pMesh->SetTexturePath(GeometryData::TextureNames::ALBEDO, "../textures/cube_texture2.png");
  pMesh->SetTexturePath(GeometryData::TextureNames::NORMAL, "../textures/cube_texture2_n.png");
  pMesh->SetTextureCoords(GeometryData::TextureNames::ALBEDO, vTexCoords.size() * 2, pfTexCoords);
  pMesh->SetTextureCoords(GeometryData::TextureNames::NORMAL, vTexCoords.size() * 2, pfTexCoords);


  delete[] pfVertices;
  delete[] pfNormals;
  delete[] pfTexCoords;
  delete[] pnIndices;

  return spNewObject;
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

