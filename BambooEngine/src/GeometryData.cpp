/*
 * source file for GeometryData
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#include "GeometryData.h"
#include "assert.h"

#include <algorithm>
#include <iostream>

const char * GeometryData::GenericData::DATA_VERTICES   = "vertices";
const char * GeometryData::GenericData::DATA_NORMALS    = "normals";
const char * GeometryData::GenericData::DATA_COLORS     = "colors";
const char * GeometryData::GenericData::DATA_TANGENTS   = "tangents";
const char * GeometryData::GenericData::DATA_BITANGENTS = "bitangents";
const char * GeometryData::GenericData::DATA_TEXCOORDS0 = "texcoords0";
const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_DIFFUSE         = "material_color_diffuse";
const char * GeometryData::GenericData::DATA_MATERIAL_COLOR_SPECULAR        = "material_color_specular";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS             = "material_shininess";
const char * GeometryData::GenericData::DATA_MATERIAL_SHININESS_STRENGTH    = "material_shininess_strength";


void GeometryData::GenericMesh::SetAttribute(const char *szName,
                                             unsigned int nNumEntries,
                                             float *pArray)
{
    // assert that this attribute was not set before
    assert (m_mAttributeMap.find(szName) == m_mAttributeMap.end());
    assert (m_mAttributeSizes.find(szName) == m_mAttributeSizes.end());

    assert (nNumEntries > 0);

    // allocate new float array
    float *pPuffer = new float[nNumEntries];

    // copy data
    for (unsigned int i=0; i < nNumEntries; i++)
        pPuffer[i] = pArray[i];

    // insert data in maps
    m_mAttributeMap[szName]     = pPuffer;
    m_mAttributeSizes[szName]   = nNumEntries;


    // TODO: the underlying data structure of GenericMesh should be changed
    // TODO: in a way that all values of one attribute, even from different
    // TODO: objects, are in a big float array (cache efficiency!), and the
    // TODO: GenericMesh holds pointers to the corresponding part of that big
    // TODO: array (so the interface must stay the same!)
    // TODO: ----
    // TODO: furthermore, in the current implementation, there may occur memory
    // TODO: leaks if an attribute is set although it was set already, because
    // TODO: the float* pointer is overwritten and the allocated memory is never
    // TODO: destroyed
}

float * GeometryData::GenericMesh::GetAttribute(const char *szName,
                                                unsigned int &rnNumEntries)
{
    auto iterator_Attribute = m_mAttributeMap.find(szName);
    auto iterator_NumEntries = m_mAttributeSizes.find(szName);

    bool bAttributeExists = ( iterator_Attribute != m_mAttributeMap.end());

    float *pfReturn = NULL;

    if (bAttributeExists)
    {
        assert (iterator_NumEntries != m_mAttributeSizes.end());

        rnNumEntries = iterator_NumEntries->second;
        pfReturn = iterator_Attribute->second;
    }
    else
        rnNumEntries = 0;

    return pfReturn;
}

void GeometryData::GenericMesh::SetIndices(unsigned int nNumEntries,
                                           unsigned int *pArray)
{
    // assert that the indices were not set before
    assert (m_pnIndices == NULL);

    // set num of entries
    m_nNumIndices = nNumEntries;

    // allocate memory
    m_pnIndices = new unsigned int [nNumEntries];

    // copy data
    for (unsigned int i=0; i < nNumEntries; i++)
        m_pnIndices[i] = pArray[i];

    // TODO: the underlying data structure of GenericMesh should be changed
    // TODO: in a way that all values of one attribute, even from different
    // TODO: objects, are in a big float array (cache efficiency!), and the
    // TODO: GenericMesh holds pointers to the corresponding part of that big
    // TODO: array (so the interface must stay the same!)
    // TODO: ----
    // TODO: furthermore, in the current implementation, there may occur memory
    // TODO: leaks if the indices are set although they were set already, because
    // TODO: the int* pointer is overwritten and the allocated memory is never
    // TODO: destroyed
}

GeometryData::GenericMesh::~GenericMesh()
{
    // delete the attribute arrays (float *, each "iter" is a std::pair<std::string, float *>)
    for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
        delete[] iter->second;


    // delete indices, if set
    if (m_pnIndices != NULL)
        delete[] m_pnIndices;
}

unsigned int * GeometryData::GenericMesh::GetIndices(unsigned int &rnNumIndices)
{
    assert (m_pnIndices != NULL);

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

void GeometryData::GenericMesh::Debug()
{
    for (auto iter=m_mAttributeMap.begin(); iter != m_mAttributeMap.end(); iter++)
    {
        std::cout << std::endl << "ATTRIBUTE #" << iter->first << "#" << std::endl;

        float *pArray = iter->second;

        for (int i=0; i < m_mAttributeSizes[iter->first]; i++)
        {
            std::cout << pArray[i] << " ";
        }
    }
}
