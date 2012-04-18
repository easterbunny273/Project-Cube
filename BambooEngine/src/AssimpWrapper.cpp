/*
 * source file for AssimpWrapper
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 11/2011 Project "BambooEngine"
  */

#include "AssimpWrapper.h"
#include "PC_Logger.h"

#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/aiMesh.h>

std::shared_ptr<GeometryData::GenericObject> AssimpWrapper::LoadModel(std::string sFilename)
{
    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE );

    const aiScene* scene = importer.ReadFile(sFilename,
                                             aiProcess_CalcTangentSpace	|
                                             aiProcess_ValidateDataStructure	|
                                             aiProcess_Triangulate		|
                                            // aiProcess_PreTransformVertices	|
                                             //aiProcess_JoinIdenticalVertices	|
                                             aiProcess_GenSmoothNormals		|
                                             aiProcess_ImproveCacheLocality |
                                             aiProcess_FindInvalidData |
                                             //aiProcess_MakeLeftHanded |
                                             //aiProcess_OptimizeMeshes |
                                             //aiProcess_OptimizeGraph  |
                                             aiProcess_GenUVCoords |
                                             aiProcess_TransformUVCoords |
                                             aiProcess_FlipUVs |
                                             aiProcess_FindDegenerates |
                                             aiProcess_SortByPType

                                             );

      // If the import failed, report it
    if( !scene)
        Logger::error() << importer.GetErrorString() << Logger::endl;
    else
        Logger::debug() << "Loaded file " << sFilename << " with assimp" << Logger::endl;

    //bool bModelHasMeshes = scene->HasMeshes();
    //bool bModelHasMaterials = scene->HasMaterials();
    //bool bModelHasTextures = scene->HasTextures();

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
       /* aiColor3D acDiffuse (0.f,0.f,0.f);
        bool bGotColorDiffuse = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, acDiffuse));
        float pfDiffuseColor[3] = { acDiffuse.r, acDiffuse.g, acDiffuse.b };
        if (bGotColorDiffuse)
        {
           // pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_MATERIAL_COLOR_DIFFUSE, 3, pfDiffuseColor);

            unsigned int nNumVertices = pMesh->mNumVertices;

            std::vector<float> vfFakedColor;

            //get vertices
            for (unsigned int ii=0; ii < nNumVertices; ii++)
            {
                vfFakedColor.push_back(pfDiffuseColor[0]);
                vfFakedColor.push_back(pfDiffuseColor[1]);
                vfFakedColor.push_back(pfDiffuseColor[2]);
            }

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_COLORS,
                                       3 * nNumVertices,
                                       &vfFakedColor[0]);
        }

        aiColor3D acSpecular (0.f, 0.f, 0.f);
        bool bGotSpecularColor = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_COLOR_SPECULAR, acSpecular));
        float pfSpecularColor[3] = { acSpecular.r, acSpecular.g, acSpecular.b };
        if (bGotSpecularColor)
            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_MATERIAL_COLOR_SPECULAR, 3, pfSpecularColor);

        float fShininess = 0.0f;
        bool bGotShininess = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_SHININESS, fShininess));
        if (bGotShininess)
            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_MATERIAL_SHININESS, 1, &fShininess);

        float fShininessStrength = 0.0f;
        bool bGotShininessStrength = (AI_SUCCESS == pUsedMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, fShininessStrength));
        if (bGotShininessStrength)
            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_MATERIAL_SHININESS_STRENGTH, 1, &fShininessStrength);*/

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

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_VERTICES,
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

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_NORMALS,
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

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_TANGENTS,
                                       3 * nNumVertices,
                                       &vfTangents[0]);

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_BITANGENTS,
                                       3 * nNumVertices,
                                       &vfBitangents[0]);
        }


        if (pMesh->GetNumColorChannels() > 1)
            Logger::error() << "The model " << sFilename << " has more than one color channel. Only 1 color channel is imported by AssimpWrapper" << Logger::endl;

        if (pMesh->HasVertexColors(0))
        {
            unsigned int nNumVertices = pMesh->mNumVertices;

            std::vector<float> vfColors;

            //get vertices
            for (unsigned int ii=0; ii < nNumVertices; ii++)
            {
                vfColors.push_back(pMesh->mColors[0][ii].r);
                vfColors.push_back(pMesh->mColors[0][ii].g);
                vfColors.push_back(pMesh->mColors[0][ii].b);
            }

            pGenericMesh->AddAttributeValues(GeometryData::GenericData::DATA_COLORS,
                                               3 * nNumVertices,
                                               &vfColors[0]);
        }

        // get indices

        unsigned int nNumFaces = pMesh->mNumFaces;
        std::vector<unsigned int> vIndicesVector;

        for (unsigned int ii=0; ii < nNumFaces; ii++)
        {
            assert (pMesh->mFaces[ii].mNumIndices == 3);

            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[0]);
            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[1]);
            vIndicesVector.push_back(pMesh->mFaces[ii].mIndices[2]);
        }

        pGenericMesh->AddIndices(nNumFaces * 3, &vIndicesVector[0]);

        if (sFilename == "models/pool_sphere.dae")
        {
            pGenericMesh->SetTexturePath(GeometryData::TextureNames::CUBEMAP, "abc");
        }

        // get textures
        for (int iTextureType= (int) aiTextureType_DIFFUSE; iTextureType <= (int) aiTextureType_UNKNOWN; iTextureType++)
        {
            int iCount = pUsedMaterial->GetTextureCount((aiTextureType) iTextureType);

            if (iCount >= 1)
            {
                aiString sTexturePath;

                if (iCount > 1)
                    Logger::debug() << "Model \"" << sFilename << "\" contains more than one texture per type. This is not supported yet." << Logger::endl;

                pUsedMaterial->GetTexture((aiTextureType) iTextureType, 0, &sTexturePath);

                GeometryData::TextureType tTextureType;

                bool bIgnoreTexture = false;

                // if the texture type is supported by GeometryData, set the corresponding type and path
                switch (iTextureType)
                {
                case aiTextureType_DIFFUSE:
                    Logger::debug() << "diffuse " << std::string(sTexturePath.data) << Logger::endl;
                    tTextureType = GeometryData::TextureNames::ALBEDO;
                    break;
                case aiTextureType_NORMALS:
                    Logger::debug() << "normals " << std::string(sTexturePath.data) << Logger::endl;
                    tTextureType = GeometryData::TextureNames::NORMAL;
                    break;
                case aiTextureType_SPECULAR:
                    Logger::debug() << "specular " << std::string(sTexturePath.data) << Logger::endl;
                    tTextureType = GeometryData::TextureNames::SPECULAR;
                    break;
                case aiTextureType_HEIGHT:
                    Logger::debug() << "height " << std::string(sTexturePath.data) << Logger::endl;
                    tTextureType = GeometryData::TextureNames::NORMAL;
                    break;
                case aiTextureType_DISPLACEMENT:
                    Logger::debug() << "displacement " << std::string(sTexturePath.data) << Logger::endl;
                    tTextureType = GeometryData::TextureNames::DISPLACE;
                    break;
                default:
                    bIgnoreTexture = true;
                }

                if (!bIgnoreTexture)
                {
                    std::vector<float> vTextureCoords;

                    if (pMesh->GetNumUVChannels() > 1)
                        Logger::error() << "Model \"" << sFilename << "\" contains more than one uv channel. That's not supported by the importer yet." << Logger::endl;

                    if (pMesh->GetNumUVChannels() >= 1)
                    {
                        for (unsigned int nVertex=0; nVertex < pMesh->mNumVertices; nVertex++)
                        {
                            vTextureCoords.push_back(pMesh->mTextureCoords[0][nVertex].x);
                            vTextureCoords.push_back(pMesh->mTextureCoords[0][nVertex].y);
                        }

                        pGenericMesh->SetTextureCoords(tTextureType, vTextureCoords.size(), &vTextureCoords[0]);
                    }

                    pGenericMesh->SetTexturePath(tTextureType, std::string(sTexturePath.data));
                }
                else
                {
                    Logger::debug() << "Model \"" << sFilename << "\" contains some types of textures which are not supported by the importer yet" << Logger::endl;
                }
            }
        }
    }


    return spObject;
}
