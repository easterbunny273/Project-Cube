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
#include <set>
#include <memory>

class GeometryData
{
public:
    typedef const char * TextureType;

    enum TGenericData
    {
      TDATA_VERTICES,
      TDATA_COLORS,
      TDATA_TANGENTS,
      TDATA_BITANGENTS
    };

    class GenericData
    {
    public:
        static const char * DATA_VERTICES;
        static const char * DATA_COLORS;
        static const char * DATA_NORMALS;
        static const char * DATA_TANGENTS;
        static const char * DATA_BITANGENTS;
        static const char * DATA_TEXCOORDS[8];
        static const char * DATA_MODELMATRIX;

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

   /* class IMesh_Vertices
    {
      virtual float *GetVertices(unsigned int &rnNumEntries) = 0;
    };
*/

    class GenericMesh //: public IMesh_Vertices
    {
      friend class GenericMesh_VerticesFacade;

    public:
        GenericMesh();
        ~GenericMesh();

        unsigned int * GetIndices(unsigned int &rnNumIndices);

        float *GetAttribute(TGenericData tData);


        std::vector<std::pair<TGenericData, float *> >  GetAttributeList();
        std::set<TGenericData> GetUsedAttributes();

        void AddAttributeValues(const char *szName, unsigned int nNumEntries, float *pArray);

        float * AllocForAttribute(TGenericData tData, unsigned int nNumEntries);
        unsigned int *AllocForIndices(unsigned int nNumIndices);

        void    AddIndices(unsigned int nNumEntries, unsigned int * pArray);
        void    SetTexturePath(TextureType tTextureType, std::string sTexturePath);

        float * GetTextureCoords(TextureType tTextureType);
        std::string GetAttributeString(TGenericData tData);
        void    SetTextureCoords(TextureType tTextureType, unsigned int nNumEntries, float *pArray);

        void  SetModelMatrix(float *pfMatrix);
        float *GetModelMatrix();

        void SetVisibility(bool bVisibility) { m_bVisible = bVisibility; }
        bool GetVisibility() { return m_bVisible; }

        std::string GetTexturePath(TextureType tTextureType);

        unsigned int NumAttributes() const;
        unsigned int NumTexCoords() const;
        unsigned int NumVertices() const;
        unsigned int NumIndices() const;

        void Debug();

    private:
        std::map<TGenericData, unsigned int> m_mAttributeOffsetsInBigMap;
        std::map<GeometryData::TGenericData, float*> m_mAttributeMap;

        std::vector<float>  m_vfMatrix;

        std::string         m_sTexturePath;

        std::map<std::string, std::vector<float> >  m_mTextureCoords;
        std::map<std::string, std::string>          m_mTexturePaths;

        int m_iMatrixOffset;
        unsigned int m_nNumVertices;
        unsigned int m_nNumIndices;

        unsigned int *m_pnIndices;

        bool m_bVisible;
    };

    class GenericMesh_VerticesFacade
    {
      friend class GenericMesh;

    private:
      GenericMesh *m_pActualObject;

    public:
      GenericMesh_VerticesFacade(GenericMesh *pActualObject);

      unsigned int NumVertices() { return m_pActualObject->NumVertices(); }
      float * GetVertices() { return m_pActualObject->GetAttribute(TDATA_VERTICES); }
      float * GetModelMatrix() { return m_pActualObject->GetModelMatrix(); }

      void SetVisibility(bool bVisible) { m_pActualObject->SetVisibility(bVisible); }
      bool GetVisibility() { return m_pActualObject->GetVisibility(); }
    };

    class GenericObject
    {
    public:
        GenericObject(unsigned int nNumMeshes);

        unsigned int NumMeshes() const;
        std::weak_ptr<GenericMesh> GetMesh(unsigned int nIndex);
        GenericMesh *GetMeshPtr(unsigned int nIndex);

    private:
        std::vector<std::shared_ptr<GenericMesh> >    m_vspMeshes;
    };
};

#endif
