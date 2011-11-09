/*
 * header file for GeometryData
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#pragma once

#ifndef __BAMBOO_GEOMETRY_DATA_HEADER

#include <map>
#include <string>
#include <vector>
#include <memory>

class GeometryData
{
public:
    class GenericData
    {
    public:
        static const char * DATA_VERTICES;
        static const char * DATA_COLORS;
        static const char * DATA_NORMALS;
        static const char * DATA_TANGENTS;
        static const char * DATA_BITANGENTS;
        static const char * DATA_TEXCOORDS0;

        static const char * DATA_MATERIAL_COLOR_DIFFUSE;
        static const char * DATA_MATERIAL_COLOR_SPECULAR;
        static const char * DATA_MATERIAL_SHININESS;
        static const char * DATA_MATERIAL_SHININESS_STRENGTH;
    };

    class GenericMesh
    {
    public:
        ~GenericMesh();

        unsigned int * GetIndices(unsigned int &rnNumIndices);

        float *GetAttribute(const char *szName, unsigned int &rnNumEntries);

        void    SetAttribute(const char *szName, unsigned int nNumEntries, float * pArray);
        void    SetIndices(unsigned int nNumEntries, unsigned int * pArray);

        void Debug();

    private:
        std::map<std::string, float *>      m_mAttributeMap;
        std::map<std::string, unsigned int> m_mAttributeSizes;

        unsigned int *m_pnIndices;
        unsigned int m_nNumIndices;
    };

    class GenericObject
    {
    public:
        GenericObject(unsigned int nNumMeshes);

        unsigned int NumMeshes() const;
        std::weak_ptr<GenericMesh> GetMesh(unsigned int nIndex);

    private:
        std::vector<std::shared_ptr<GenericMesh> >    m_vspMeshes;
    };

};

#endif
