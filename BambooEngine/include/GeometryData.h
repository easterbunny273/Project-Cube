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
    typedef const char * TextureType;

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
        static TextureType ALBEDO;
        static TextureType SPECULAR;
        static TextureType NORMAL;
        static TextureType DISPLACE;
        static TextureType CUBEMAP;
    };

    class GenericMesh
    {
    public:
        GenericMesh();
        ~GenericMesh();

        unsigned int * GetIndices(unsigned int &rnNumIndices);

        float *GetAttribute(const char *szName, unsigned int &rnNumEntries);

        std::vector<std::pair<std::string, std::vector<float> > > GetAttributeList();

        void    AddAttributeValues(const char *szName, unsigned int nNumEntries, float * pArray);
        void    AddIndices(unsigned int nNumEntries, unsigned int * pArray);
        void    SetTexturePath(TextureType tTextureType, std::string sTexturePath);

        float * GetTextureCoords(TextureType tTextureType);
        void    SetTextureCoords(TextureType tTextureType, unsigned int nNumEntries, float *pArray);

        std::string GetTexturePath(TextureType tTextureType);

        unsigned int NumAttributes() const;
        unsigned int NumTexCoords() const;
        unsigned int NumVertices() const;

        void Debug();

    private:
        std::map<std::string, std::vector<float> >      m_mAttributeMap;

        std::string         m_sTexturePath;

        std::map<std::string, std::vector<float> >  m_mTextureCoords;
        std::map<std::string, std::string>          m_mTexturePaths;

        unsigned int m_nNumVertices;

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
