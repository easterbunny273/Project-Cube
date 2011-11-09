/*
 * source file for AssimpWrapper
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#include "AssimpWrapper.h"
#include "Logger.h"

#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/aiMesh.h>

std::shared_ptr<GeometryData::GenericObject> AssimpWrapper::LoadModel(std::string sFilename)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(sFilename,
                                             aiProcess_CalcTangentSpace	|
                                             //aiProcess_ValidateDataStructure	|
                                             //aiProcess_Triangulate		|
                                             aiProcess_PreTransformVertices	|
                                             //aiProcess_JoinIdenticalVertices	|
                                             aiProcess_GenNormals		|
                                             aiProcess_ImproveCacheLocality |
                                             //aiProcess_FindInvalidData |
                                             //aiProcess_OptimizeMeshes |
                                             //aiProcess_OptimizeGraph  |
                                            // aiProcess_FindDegenerates |
                                             aiProcess_SortByPType);

      // If the import failed, report it
      if( !scene)
        Logger::error() << importer.GetErrorString() << Logger::endl;
    else
        Logger::debug() << "Loaded file " << sFilename << " with assimp" << Logger::endl;

    bool bModelHasMeshes = scene->HasMeshes();
    bool bModelHasMaterials = scene->HasMaterials();
    bool bModelHasTextures = scene->HasTextures();

    unsigned int nNumMeshes = scene->mNumMeshes;

    assert (nNumMeshes > 0);

    GeometryData::GenericObject *pObject = new GeometryData::GenericObject(nNumMeshes);

    std::shared_ptr<GeometryData::GenericObject> spObject(pObject);


    for (unsigned int i=0; i < nNumMeshes; i++)
    {
        aiMesh *pMesh = scene->mMeshes[i];
        GeometryData::GenericMesh *pGenericMesh = std::shared_ptr<GeometryData::GenericMesh>(pObject->GetMesh(i)).get();

        unsigned int nMaterialIndex = pMesh->mMaterialIndex;

        aiMaterial *pUsedMaterial = scene->mMaterials[nMaterialIndex];

        // now fetch the material properties
        aiColor3D acDiffuse (0.f,0.f,0.f);
        bool bGotColorDiffuse = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, acDiffuse));
        float pfDiffuseColor[3] = { acDiffuse.r, acDiffuse.g, acDiffuse.b };
        pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_MATERIAL_COLOR_DIFFUSE, 3, pfDiffuseColor);

        aiColor3D acSpecular (0.f, 0.f, 0.f);
        bool bGotSpecularColor = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_COLOR_SPECULAR, acSpecular));
        float pfSpecularColor[3] = { acSpecular.r, acSpecular.g, acSpecular.b };
        pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_MATERIAL_COLOR_SPECULAR, 3, pfSpecularColor);

        float fShininess = 0.0f;
        bool bGotShininess = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_SHININESS, fShininess));
        pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_MATERIAL_SHININESS, 1, &fShininess);

        float fShininessStrength = 0.0f;
        bool bGotShininessStrength = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, fShininessStrength));
        pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_MATERIAL_SHININESS_STRENGTH, 1, &fShininessStrength);

        assert (pMesh->HasPositions());

        if (pMesh->HasPositions())
        {
            // get vertices
            unsigned int nNumVertices = pMesh->mNumVertices;

            std::vector<float> vfVertices;

            //get vertices
            for (unsigned int ii=0; ii < nNumVertices; ii++)
            {
                vfVertices.push_back(pMesh->mVertices[ii].x);
                vfVertices.push_back(pMesh->mVertices[ii].y);
                vfVertices.push_back(pMesh->mVertices[ii].z);
            }

            pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_VERTICES,
                                       3 * nNumVertices,
                                       &vfVertices[0]);
        }


        if (pMesh->HasNormals())
        {
            unsigned int nNumVertices = pMesh->mNumVertices;

            std::vector<float> vfNormals;

            //get vertices
            for (unsigned int ii=0; ii < nNumVertices; ii++)
            {
                vfNormals.push_back(pMesh->mNormals[ii].x);
                vfNormals.push_back(pMesh->mNormals[ii].y);
                vfNormals.push_back(pMesh->mNormals[ii].z);
            }

            pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_NORMALS,
                                       3 * nNumVertices,
                                       &vfNormals[0]);
        }

        if (pMesh->HasTangentsAndBitangents())
        {
            unsigned int nNumVertices = pMesh->mNumVertices;

            std::vector<float> vfTangents;
            std::vector<float> vfBitangents;

            //get vertices
            for (unsigned int ii=0; ii < nNumVertices; ii++)
            {
                vfTangents.push_back(pMesh->mTangents[ii].x);
                vfTangents.push_back(pMesh->mTangents[ii].y);
                vfTangents.push_back(pMesh->mTangents[ii].z);
                vfBitangents.push_back(pMesh->mBitangents[ii].x);
                vfBitangents.push_back(pMesh->mBitangents[ii].y);
                vfBitangents.push_back(pMesh->mBitangents[ii].z);
            }

            pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_TANGENTS,
                                       3 * nNumVertices,
                                       &vfTangents[0]);

            pGenericMesh->SetAttribute(GeometryData::GenericData::DATA_BITANGENTS,
                                       3 * nNumVertices,
                                       &vfTangents[0]);
        }

        // get indices

        unsigned int nNumFaces = pMesh->mNumFaces;
        std::vector<unsigned int> vIndicesVector;

        for (int ii=0; ii < nNumFaces; ii++)
        {
            assert (pMesh->mFaces[ii].mNumIndices == 3);

            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[0]);
            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[1]);
            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[2]);
        }

        pGenericMesh->SetIndices(nNumFaces * 3, &vIndicesVector[0]);
    }


    return spObject;
}
