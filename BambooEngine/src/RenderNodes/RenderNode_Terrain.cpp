#include "RenderNodes/RenderNode_Terrain.h"

Bamboo::RN_Terrain::RN_Terrain(std::string sFilename)
{
  m_sFilename = sFilename;

  m_nVertexArrayObject = 0;
  m_nVertexBufferObject = 0;
  m_nVertexIndexObject = 0;

  ItlCreateBuffers();
}

Bamboo::RN_Terrain::~RN_Terrain()
{

}


void Bamboo::RN_Terrain::ItlCreateBuffers()
{
  unsigned int iN = 512;

  std::vector<GLfloat> vVertexBuffer;

  for (unsigned int i=0; i < iN; i++)
    for (unsigned int j=0; j < iN; j++)
      {
        float fX = i / (float) iN - 0.5;
        float fZ = j / (float) iN - 0.5;

        // vertex
        vVertexBuffer.push_back(fX);
        vVertexBuffer.push_back(0.0f);
        vVertexBuffer.push_back(fZ);

        // texcoords
        vVertexBuffer.push_back(i / (float) iN);
        vVertexBuffer.push_back(j / (float) iN);
      }


  std::vector<GLuint> vIndices;

  for (unsigned int iX = 0; iX < iN-1; iX++)
    for (unsigned int iY = 0; iY < iN-1; iY++)
      {
        vIndices.push_back(iY * iN + iX);
        vIndices.push_back(iY * iN + iX + 1);
        vIndices.push_back((iY+1) * iN + iX);

        vIndices.push_back((iY+1) * iN + iX);
        vIndices.push_back(iY * iN + iX + 1);
        vIndices.push_back((iY+1) * iN + iX + 1);
      }

  glGenBuffers(1, &m_nVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, vVertexBuffer.size() * sizeof(GLfloat), &(vVertexBuffer[0]), GL_STATIC_DRAW);

  glGenBuffers(1, &m_nVertexIndexObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nVertexIndexObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLuint), &(vIndices[0]), GL_STATIC_DRAW);


}


void Bamboo::RN_Terrain::ItlPreRenderChildren()
{

}

void Bamboo::RN_Terrain::ItlPostRenderChildren()
{

}

void Bamboo::RN_Terrain::ItlPreRender()
{

}

void Bamboo::RN_Terrain::ItlPostRender()
{

}

void Bamboo::RN_Terrain::ItlRender()
{

}

bool Bamboo::RN_Terrain::ItlTestSkipRendering()
{

}
