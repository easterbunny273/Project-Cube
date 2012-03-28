/*
 * source file for GeometryData
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#include "GeometryData.h"
#include "assert.h"

#include <algorithm>
#include <iostream>

#include <cstdlib>
#include <cstdio>
#include <cstring>

const char * GeometryData::GenericData::DATA_VERTICES   = "in_Position";
const char * GeometryData::GenericData::DATA_NORMALS    = "in_Normal";
const char * GeometryData::GenericData::DATA_COLORS     = "in_Color";
const char * GeometryData::GenericData::DATA_TANGENTS   = "in_Tangent";
const char * GeometryData::GenericData::DATA_BITANGENTS = "in_Bitangent";

const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_DIFFUSE         = "material_color_diffuse";
const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_SPECULAR        = "material_color_specular";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS             = "material_shininess";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS_STRENGTH    = "material_shininess_strength";

GeometryData::TextureType GeometryData::TextureNames::ALBEDO    = "color_texture";
GeometryData::TextureType GeometryData::TextureNames::NORMAL    = "normal_texture";
GeometryData::TextureType GeometryData::TextureNames::SPECULAR  = "specular_texture";
GeometryData::TextureType GeometryData::TextureNames::DISPLACE  = "displace_texture";
GeometryData::TextureType GeometryData::TextureNames::CUBEMAP  = "cubemap_texture";

GeometryData::GenericMesh::GenericMesh()
{
    m_nNumVertices = 0;
    m_pnIndices = NULL;
    m_bVisible = true;
}

std::map<GeometryData::TGenericData, std::vector<float> > mBigArrays;
std::vector<unsigned int> mBigIndexArray;

void GeometryData::GenericMesh::AddAttributeValues(const char *szName,
                                             unsigned int nNumEntries,
                                             float *pArray)
{
  assert(!"do not use this method in rendering-ue-branch");
}

float *GeometryData::GenericMesh::AllocForAttribute(GeometryData::TGenericData tData, unsigned int nNumEntries)
{
  unsigned int nOldSize = mBigArrays[tData].size();
  unsigned int nNewSize = nOldSize + nNumEntries;

  assert (nNewSize < mBigArrays[tData].max_size());

  //std::cout << "resize attribute " << szName << " to " << nNewSize << std::endl;

  mBigArrays[tData].resize(nNewSize);

  float *pfAllocatedArray = &(mBigArrays[tData][nOldSize]);

  m_mAttributeOffsetsInBigMap[tData] = nOldSize;

  assert (nNumEntries % 3 == 0);
  m_nNumVertices = nNumEntries / 3;

  return pfAllocatedArray;
}

unsigned int *GeometryData::GenericMesh::AllocForIndices(unsigned int nNumIndices)
{
  assert (m_pnIndices == NULL);

  /*unsigned int nOldSize = mBigIndexArray.size();
  unsigned int nNewSize = nOldSize + nNumIndices;

  assert (nNewSize < mBigIndexArray.max_size());

  mBigIndexArray.resize(nNewSize);*/

  m_pnIndices = new unsigned int[nNumIndices];// &mBigIndexArray[nOldSize];

  m_nNumIndices = nNumIndices;

  return m_pnIndices;
}

float * GeometryData::GenericMesh::GetAttribute(GeometryData::TGenericData tData)
{
    auto iterator_Attribute = m_mAttributeOffsetsInBigMap.find(tData);

    bool bAttributeExists = ( iterator_Attribute != m_mAttributeOffsetsInBigMap.end());

    float *pfReturn = NULL;

    unsigned int nOffsetInBigMem = m_mAttributeOffsetsInBigMap[tData];

    assert (mBigArrays.find(tData) != mBigArrays.end());

    if (bAttributeExists)
      pfReturn = &(mBigArrays[tData][nOffsetInBigMem]);
            //pfReturn =  m_mAttributeMap[szName];

    return pfReturn;
}

void GeometryData::GenericMesh::AddIndices(unsigned int nNumEntries,
                                           unsigned int *pArray)
{
    assert (!" should not be used in rendering-ue!");

    AllocForIndices(nNumEntries);

    assert (m_pnIndices != NULL);

    memcpy(m_pnIndices, pArray, nNumEntries * sizeof(unsigned int));
}

GeometryData::GenericMesh::~GenericMesh()
{

}

unsigned int * GeometryData::GenericMesh::GetIndices(unsigned int &rnNumIndices)
{
    rnNumIndices = m_nNumIndices;

    return m_pnIndices;
}

GeometryData::GenericObject::GenericObject(unsigned int nNumMeshes)
{
    m_vspMeshes.resize(nNumMeshes);

    for (unsigned int i=0; i < nNumMeshes; i++)
        m_vspMeshes[i] = std::shared_ptr<GeometryData::GenericMesh>(new GeometryData::GenericMesh());
}

unsigned int GeometryData::GenericObject::NumMeshes() const
{
    return m_vspMeshes.size();
}

std::weak_ptr<GeometryData::GenericMesh> GeometryData::GenericObject::GetMesh(unsigned int nIndex)
{
  return std::weak_ptr<GeometryData::GenericMesh>(m_vspMeshes[nIndex]);
}

GeometryData::GenericMesh *GeometryData::GenericObject::GetMeshPtr(unsigned int nIndex)
{
  return m_vspMeshes[nIndex].get();
}

void GeometryData::GenericMesh::Debug()
{
  assert (!"not implemented");

 /*   for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
    {
        std::cout << std::endl << "ATTRIBUTE #" << iter->first << "#" << std::endl;

        float *pArray = &(iter->second[0]);

        for (unsigned int i=0; i < iter->second.size(); i++)
        {
            std::cout << pArray[i] << " ";
        }
    }*/
}

void GeometryData::GenericMesh::SetTexturePath(TextureType tTextureType,
                                               std::string sTexturePath)
{
    m_mTexturePaths[std::string(tTextureType)] = sTexturePath;
}

unsigned int GeometryData::GenericMesh::NumAttributes() const
{
  return m_mAttributeOffsetsInBigMap.size();
}

std::vector<std::pair<GeometryData::TGenericData, float *> > GeometryData::GenericMesh::GetAttributeList()
{
    std::vector<std::pair<GeometryData::TGenericData, float *> > vAttributeList;

    for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
    {
        vAttributeList.push_back(std::pair<GeometryData::TGenericData, float *>(iter->first, iter->second));
    }

    return vAttributeList;
}

std::set<GeometryData::TGenericData> GeometryData::GenericMesh::GetUsedAttributes()
{
  std::set<GeometryData::TGenericData> vAttributeList;

  for (auto iter=m_mAttributeOffsetsInBigMap.begin(); iter != m_mAttributeOffsetsInBigMap.end(); iter++)
  {
      vAttributeList.insert(iter->first);
  }

  return vAttributeList;
}

std::string GeometryData::GenericMesh::GetTexturePath(GeometryData::TextureType tTextureType)
{
    // find the requested entry in m_mTexturePaths
    auto iter = m_mTexturePaths.find(tTextureType);

    // if found, return the texture path (the iter points to a pair of TextureType/TexturePath), else return an empty string
    return (iter != m_mTexturePaths.end()) ? iter->second : std::string();
}

unsigned int GeometryData::GenericMesh::NumTexCoords() const
{
    assert (m_mTextureCoords.size() == m_mTexturePaths.size());

    return m_mTextureCoords.size();
}

float * GeometryData::GenericMesh::GetTextureCoords(GeometryData::TextureType tTextureType)
{
    // find the requested entry in m_mTextureCoords
    auto iter = m_mTextureCoords.find(tTextureType);

    float *pReturn = NULL;

    if (iter != m_mTextureCoords.end())
        pReturn = &(iter->second[0]);

    return pReturn;

}

std::string GeometryData::GenericMesh::GetAttributeString(GeometryData::TGenericData tData)
{
  switch (tData)
    {
    case TDATA_VERTICES:
      return std::string(GeometryData::GenericData::DATA_VERTICES);
    case TDATA_TANGENTS:
      return std::string(GeometryData::GenericData::DATA_VERTICES);
    default:
      assert(!"not implemented yet - just for rendering ue");

    }
}

void GeometryData::GenericMesh::SetTextureCoords(GeometryData::TextureType tTextureType, unsigned int nNumEntries, float *pArray)
{
    assert (nNumEntries > 0);
    if (m_nNumVertices == 0)
        m_nNumVertices = (nNumEntries / 2);
    else
        assert (m_nNumVertices == (nNumEntries / 2));

    for (int i=0; i < nNumEntries; i++)
      m_mTextureCoords[tTextureType].push_back(pArray[i]);
}

void GeometryData::GenericMesh::SetModelMatrix(float *pfMatrix)
{
  m_vfMatrix.resize(16);

  for (unsigned int i=0; i < 16; i++)
    m_vfMatrix[i] = pfMatrix[i];


}

float *GeometryData::GenericMesh::GetModelMatrix()
{
  return &(m_vfMatrix[0]);
}

unsigned int GeometryData::GenericMesh::NumVertices() const
{
  return m_nNumVertices;
}

unsigned int GeometryData::GenericMesh::NumIndices() const
{
  return m_nNumIndices;
}

GeometryData::GenericMesh_VerticesFacade::GenericMesh_VerticesFacade(GeometryData::GenericMesh *pActualObject)
{
  m_pActualObject = pActualObject;
}

/*
void GeometryData::PerVertexData::AddVertices(unsigned int nNumVertices,
                                              float *pPositions,
                                              float *pColors,
                                              float *pNormals,
                                              float *pTangents,
                                              float *pBitangents)
{
    int iOldSize = m_vColors.size();

    m_vColors.reserve(m_vColors.size() + nVertices);
    m_vNormals.reserve(m_vNormals.size() + nVertices);
    m_vPositions.reserve(m_vPositions.size() + nVertices);
    m_vBitangents.reserve(m_vBitangents.size() + nVertices);
    m_vTangents.reserve(m_vTangents.size() + nVertices);

    unsigned int nVerticesLeft = nNumVertices;

    while (nVerticesLeft--)
    {
        m_vPositions.push_back(*pPositions);
        m_vNormals.push_back(*pNormals);
        m_vColors.push_back(*pColors);
        m_vBitangents.push_back(*pBitangents);
        m_vTangents.push_back(*pTangents);

        pPositions++;
        pNormals++;
        pColors++;
        pBitangents++;
        pTangents++;
    }

    assert (iOldSize + nNumVertices == m_vColors.size());
}

void GeometryData::PerVertexData::AddIndices(unsigned int nNumIndices, unsigned int *pIndices)
{

}*/
