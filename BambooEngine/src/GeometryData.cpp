/*
 * source file for GeometryData
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#include "GeometryData.h"
#include "assert.h"

#include <algorithm>
#include <iostream>

const char * GeometryData::GenericData::DATA_VERTICES   = "in_Position";
const char * GeometryData::GenericData::DATA_NORMALS    = "in_Normal";
const char * GeometryData::GenericData::DATA_COLORS     = "in_Colors";
const char * GeometryData::GenericData::DATA_TANGENTS   = "in_Tangents";
const char * GeometryData::GenericData::DATA_BITANGENTS = "in_Bitangents";
const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_DIFFUSE         = "material_color_diffuse";
const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_SPECULAR        = "material_color_specular";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS             = "material_shininess";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS_STRENGTH    = "material_shininess_strength";

const char * GeometryData::TextureNames::DIFFUSE_COLOR = "color_texture";

void GeometryData::GenericMesh::AddAttributeValues(const char *szName,
                                             unsigned int nNumEntries,
                                             float *pArray)
{
    // assert that this attribute was not set before
    assert (nNumEntries > 0);

    std::vector<float> * pfValues = &(m_mAttributeMap[szName]);

    // allocate new float array
    pfValues->reserve(pfValues->size() + nNumEntries);

    // copy data
    for (unsigned int i=0; i < nNumEntries; i++)
        pfValues->push_back(pArray[i]);

    // TODO: the underlying data structure of GenericMesh should be changed
    // TODO: in a way that all values of one attribute, even from different
    // TODO: objects, are in a big float array (cache efficiency!), and the
    // TODO: GenericMesh holds pointers to the corresponding part of that big
    // TODO: array (so the interface must stay the same!)
}

float * GeometryData::GenericMesh::GetAttribute(const char *szName,
                                                unsigned int &rnNumEntries)
{
    auto iterator_Attribute = m_mAttributeMap.find(szName);

    bool bAttributeExists = ( iterator_Attribute != m_mAttributeMap.end());

    float *pfReturn = NULL;

    if (bAttributeExists)
    {

        std::vector<float> *pfVector = &(m_mAttributeMap[szName]);

        rnNumEntries = pfVector->size();
        pfReturn = &(pfVector->at(0));
    }
    else
        rnNumEntries = 0;

    return pfReturn;
}

void GeometryData::GenericMesh::AddIndices(unsigned int nNumEntries,
                                           unsigned int *pArray)
{
    m_vIndices.reserve(m_vIndices.size() + nNumEntries);

    // allocate memory

    // copy data
    for (unsigned int i=0; i < nNumEntries; i++)
        m_vIndices.push_back(pArray[i]);

    // TODO: the underlying data structure of GenericMesh should be changed
    // TODO: in a way that all values of one attribute, even from different
    // TODO: objects, are in a big float array (cache efficiency!), and the
    // TODO: GenericMesh holds pointers to the corresponding part of that big
    // TODO: array (so the interface must stay the same!)
}

GeometryData::GenericMesh::~GenericMesh()
{

}

unsigned int * GeometryData::GenericMesh::GetIndices(unsigned int &rnNumIndices)
{
    rnNumIndices = m_vIndices.size();
    return &(m_vIndices[0]);
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

void GeometryData::GenericMesh::Debug()
{
    for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
    {
        std::cout << std::endl << "ATTRIBUTE #" << iter->first << "#" << std::endl;

        float *pArray = &(iter->second[0]);

        for (unsigned int i=0; i < iter->second.size(); i++)
        {
            std::cout << pArray[i] << " ";
        }
    }
}

void GeometryData::GenericMesh::SetTexturePath(std::string sTexturePath)
{
    assert (m_sTexturePath.empty());

    m_sTexturePath = sTexturePath;
}

unsigned int GeometryData::GenericMesh::NumAttributes() const
{
    return m_mAttributeMap.size();
}

std::vector<std::pair<std::string, std::vector<float> > > GeometryData::GenericMesh::GetAttributeList()
{
    std::vector<std::pair<std::string, std::vector<float> > > vAttributeList;

    for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
    {
        vAttributeList.push_back(std::pair<std::string, std::vector<float> >(iter->first, iter->second));
    }

    return vAttributeList;
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
