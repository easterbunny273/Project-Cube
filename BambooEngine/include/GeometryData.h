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
        static const char * DATA_TEXCOORDS[8];

        static const char * DATA_MATERIAL_COLOR_DIFFUSE;
        static const char * DATA_MATERIAL_COLOR_SPECULAR;
        static const char * DATA_MATERIAL_SHININESS;
        static const char * DATA_MATERIAL_SHININESS_STRENGTH;
    };

    class TextureNames
    {
    public:
        static const char * DIFFUSE_COLOR;
    };

    class GenericMesh
    {
    public:
        ~GenericMesh();

        unsigned int * GetIndices(unsigned int &rnNumIndices);

        float *GetAttribute(const char *szName, unsigned int &rnNumEntries);

        std::vector<std::pair<std::string, std::vector<float> > > GetAttributeList();

        void    AddAttributeValues(const char *szName, unsigned int nNumEntries, float * pArray);
        void    AddIndices(unsigned int nNumEntries, unsigned int * pArray);
        void    SetTexturePath(std::string sTexturePath);

        unsigned int NumAttributes() const;
        void Debug();

    private:
        std::map<std::string, std::vector<float> >      m_mAttributeMap;

        std::string         m_sTexturePath;

        std::map<std::string, std::vector<float> >  m_mTextureCoords;
        std::map<std::string, unsigned int>         m_mTextures;


        std::vector<unsigned int>   m_vIndices;
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
