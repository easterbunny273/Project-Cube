//general includes
#include "Logger.h"
#include "Graphics/ShaderManager.h"
#include "Graphics/TextureManager.h"

//class specific
#include "Graphics/SceneObjects/SceneObject_AssimpImport.h"
#include "Graphics/SceneObjects/SceneObject_RenderPass.h"
#include "Graphics/SceneObjects/SceneObject_BoundingBox.h"
#include "Settings.h"
#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>
#include <assimp/aiMesh.h>
#include <vector>
#include <iostream>

struct SceneObject_AssimpImport::TItlMeshData
{
    unsigned int nNumFaces;
    unsigned int nNumVertices;
    unsigned int nNumUVChannels;
    unsigned int nMaterialIndex;
    unsigned int nVboOffsetVertices;
    unsigned int nVboOffsetNormals;
    unsigned int nVboOffsetTangents;
    unsigned int nVboOffsetBitangents;
    unsigned int nVboOffsetUVCoords;
    unsigned int nIboOffsetFaces;

};

struct SceneObject_AssimpImport::TItlMaterialData
{
    float fColorDiffuse[4];
    float fColorSpecular[4];
    float fColorAmbient[4];

    /*bool bWireframe;
    bool bTwosided;*/

    float fShininess;
    float fShininess_Strength;

    std::string sTexturePath;

    bool bHasBumpTexture;
    bool bHasNormalTexture;
    bool bHasColorTexture;

    std::string sInternalLoadedColorTexture;
    std::string sInternalLoadedNormalTexture;
    std::string sInternalLoadedBumpTexture;
};

// static member initialization
bool SceneObject_AssimpImport::s_bGeneralRessourcesInitialized = false;

SceneObject_AssimpImport::SceneObject_AssimpImport(std::string sFilename)
{
    ItlLoadGeneralRessources();

    Assimp::Importer importer;
    std::vector<GLfloat> vVertexBufferData;
    std::vector<GLuint> vIndexBufferData;

    m_pfVertices = NULL;
    m_piIndices = NULL;

    m_iNumVertices = 0;
    m_iNumIndices = 0;

      // And have it read the given file with some example postprocessing
      // Usually - if speed is not the most important aspect for you - you'll
      // propably to request more postprocessing than we do in this example.
      const aiScene* scene = importer.ReadFile( sFilename,
						aiProcess_CalcTangentSpace	|
						aiProcess_ValidateDataStructure	|
						aiProcess_Triangulate		|
						aiProcess_PreTransformVertices	|
						aiProcess_JoinIdenticalVertices	|
						aiProcess_GenNormals		|
						//aiProcess_ImproveCacheLocality |
						//aiProcess_FindInvalidData |
						aiProcess_OptimizeMeshes |
						aiProcess_OptimizeGraph  |
						//aiProcess_FindDegenerates |
						aiProcess_SortByPType);

      // If the import failed, report it
      if( !scene)
      {
	Logger::error() << importer.GetErrorString() << Logger::endl;
      }
      else
      {
	Logger::debug() << "Loaded file " << sFilename << " with assimp" << Logger::endl;

	bool bModelHasMeshes = scene->HasMeshes();
//	bool bModelHasMaterials = scene->HasMaterials();
//	bool bModelHasTextures = scene->HasTextures();

	assert (bModelHasMeshes);

	int iNumMeshes = scene->mNumMeshes;

	assert (iNumMeshes > 0);

        for (unsigned int i=0; i < scene->mNumMaterials; i++)
	{
	    struct TItlMaterialData *material_data = new TItlMaterialData;
	    aiMaterial *mat = scene->mMaterials[i];

	    //get diffuse color
	    aiColor3D diffuse_color (0.f,0.f,0.f);
	    bool bOk1 = (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color));

	    material_data->fColorDiffuse[0] = diffuse_color.r;
	    material_data->fColorDiffuse[1] = diffuse_color.g;
	    material_data->fColorDiffuse[2] = diffuse_color.b;
	    material_data->fColorDiffuse[3] = 1.0f;

	    //get specular color
	    aiColor3D specular_color (0.f, 0.f, 0.f);
	    bool bOk2 = (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, specular_color));



	    material_data->fColorSpecular[0] = specular_color.r;
	    material_data->fColorSpecular[1] = specular_color.g;
	    material_data->fColorSpecular[2] = specular_color.b;
	    material_data->fColorSpecular[3] = 1.0f;

	    //get ambient color
	    aiColor3D ambient_color (0.f, 0.f, 0.f);
	    bool bOk3 = (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color));


	    material_data->fColorAmbient[0] = ambient_color.r;
	    material_data->fColorAmbient[1] = ambient_color.g;
	    material_data->fColorAmbient[2] = ambient_color.b;
	    material_data->fColorAmbient[3] = 1.0f;

	    //get shininess and strength
	    bool bOk4 = (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, material_data->fShininess));

	    bool bOk5 = (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS_STRENGTH, material_data->fShininess_Strength));


	 /*   assert (bOk1);
            assert (bOk2);
            assert (bOk3);
            assert (bOk4);
	    assert (bOk5);*/

	    //get texture
	    //first, find texture. we allow only one texture per material
	    bool bTextureFound = false;

	    for (int test= (int) aiTextureType_DIFFUSE; test <= (int) aiTextureType_UNKNOWN; test++)
	    {
		int iCount = mat->GetTextureCount((aiTextureType) test);

		if (iCount >= 1)
		{
		    assert (bTextureFound == false);
		    assert (iCount == 1);

		    aiString testPath;

		    scene->mMaterials[i]->GetTexture((aiTextureType) test, 0, &testPath);

		    std::string sTexturePath("textures/models/");
		    sTexturePath.append(testPath.data);

		    std::string sTextureNormalPath = sTexturePath + ".normals";
		    std::string sTextureBumpPath = sTexturePath + ".bump";

		    std::string sColorTextureName("AssimpImport_#");
		    sColorTextureName.append(sTexturePath);

		    std::string sNormalTextureName(sColorTextureName);
		    sNormalTextureName.append("_NORMAL");

		    std::string sBumpTextureName(sColorTextureName);
		    sBumpTextureName.append("_BUMP");

		    bool bOk = TextureManager::instance()->LoadTexture(sColorTextureName, sTexturePath, true);
		    bool bOk_Normal = TextureManager::instance()->LoadTexture(sNormalTextureName, sTextureNormalPath, false);
		    bool bOk_Bump = TextureManager::instance()->LoadTexture(sBumpTextureName, sTextureBumpPath, false);

		    if (!bOk)
			Logger::fatal() << "Could not load material for model" << Logger::endl;

		    material_data->sTexturePath = sTexturePath;

		    material_data->sInternalLoadedColorTexture = sColorTextureName;
		    material_data->sInternalLoadedBumpTexture = sBumpTextureName;
		    material_data->sInternalLoadedNormalTexture = sNormalTextureName;

		    material_data->bHasColorTexture = bOk;
		    material_data->bHasBumpTexture = bOk_Bump;
		    material_data->bHasNormalTexture = bOk_Normal;

		    bTextureFound = true;
		}
	    }

	    material_data->bHasColorTexture = bTextureFound;

	    //check if new index in m_vMaterialData will be the same as current i
            assert (m_vMaterialData.size() == static_cast<unsigned int>(i));

	    m_vMaterialData.push_back(material_data);
	}

	for (int i=0; i < iNumMeshes; i++)
	{
	    TItlMeshData *meshData = new TItlMeshData;

	    const aiMesh *pMesh = scene->mMeshes[i];

	    meshData->nMaterialIndex = pMesh->mMaterialIndex;
	    meshData->nVboOffsetVertices = vVertexBufferData.size();
	    meshData->nIboOffsetFaces = vIndexBufferData.size();

	    assert (pMesh->HasPositions());

	    int iNumFaces = pMesh->mNumFaces;

	    meshData->nNumFaces = iNumFaces;

	    for (int ii=0; ii < iNumFaces; ii++)
	    {
		assert (pMesh->mFaces[ii].mNumIndices == 3);

		vIndexBufferData.push_back(pMesh->mFaces[ii].mIndices[0]);
		vIndexBufferData.push_back(pMesh->mFaces[ii].mIndices[1]);
		vIndexBufferData.push_back(pMesh->mFaces[ii].mIndices[2]);
	    }

	    int iNumVertices = pMesh->mNumVertices;

	    meshData->nNumVertices = (unsigned int) iNumVertices;

	    //get vertices
	    for (int ii=0; ii < iNumVertices; ii++)
	    {
		vVertexBufferData.push_back(pMesh->mVertices[ii].x);
		vVertexBufferData.push_back(pMesh->mVertices[ii].y);
		vVertexBufferData.push_back(pMesh->mVertices[ii].z);
	    }

	    assert (pMesh->HasNormals());

	    meshData->nVboOffsetNormals = vVertexBufferData.size();

	    //get normals
	    for (int ii=0; ii < iNumVertices; ii++)
	    {
		vVertexBufferData.push_back(pMesh->mNormals[ii].x);
		vVertexBufferData.push_back(pMesh->mNormals[ii].y);
		vVertexBufferData.push_back(pMesh->mNormals[ii].z);
	    }

	    assert (pMesh->HasTangentsAndBitangents());

	    if (pMesh->HasTangentsAndBitangents())
	    {

		meshData->nVboOffsetTangents = vVertexBufferData.size();

		for (int ii=0; ii < iNumVertices; ii++)
		{
		    vVertexBufferData.push_back(pMesh->mTangents[ii].x);
		    vVertexBufferData.push_back(pMesh->mTangents[ii].y);
		    vVertexBufferData.push_back(pMesh->mTangents[ii].z);
		}


		meshData->nVboOffsetBitangents = vVertexBufferData.size();

		for (int ii=0; ii < iNumVertices; ii++)
		{
		    vVertexBufferData.push_back(pMesh->mBitangents[ii].x);
		    vVertexBufferData.push_back(pMesh->mBitangents[ii].y);
		    vVertexBufferData.push_back(pMesh->mBitangents[ii].z);
		}

	    }

           // unsigned int nTestSize = vVertexBufferData.size();

	    assert (pMesh->GetNumUVChannels() > 0);

	    meshData->nVboOffsetUVCoords = vVertexBufferData.size();
	    meshData->nNumUVChannels = pMesh->GetNumUVChannels();

            for (unsigned int l=0; l < meshData->nNumUVChannels; l++)
		for (int ii=0; ii < iNumVertices; ii++)
		{
		    vVertexBufferData.push_back(pMesh->mTextureCoords[l][ii].x);
		    vVertexBufferData.push_back(pMesh->mTextureCoords[l][ii].y);
		    vVertexBufferData.push_back(pMesh->mTextureCoords[l][ii].z);
		}

	    m_vMeshData.push_back(meshData);


	}

	m_nNumMeshes = m_vMeshData.size();

      // Now we can access the file's contents.
	glGenVertexArrays(1, &m_nVAO);
	glBindVertexArray(m_nVAO);
	ItlCreateVertexBufferObject(vVertexBufferData);
	ItlCreateIndexBufferObject(vIndexBufferData);
	ItlCreateVerticesArray(vVertexBufferData);
	ItlCreateIndicesArray(vIndexBufferData);
	glBindVertexArray(0);


      }

     /* std::shared_ptr<SceneObject> pBoundingSphere(new SceneObject_BoundingSphere(CalculateBoundingRadius()));
     // this->AddChild(pBoundingSphere);

      CalculateBoundingBox(m_fMinX, m_fMaxX, m_fMinY, m_fMaxY, m_fMinZ, m_fMaxZ);

      std::shared_ptr<SceneObject> pBoundingBox(new SceneObject_BoundingBox(m_fMinX, m_fMaxX, m_fMinY, m_fMaxY, m_fMinZ, m_fMaxZ));
      this->AddChild(pBoundingBox);*/
}

void SceneObject_AssimpImport::ItlCreateVertexBufferObject(std::vector<GLfloat> &data)
{
    // Vertexbuffer, Indexbuffer, Normalbuffer, texcoordbuffer generieren
    glGenBuffers(1, &m_nVBO);

    //create one big array and copy vector contents
    GLfloat *packed_vertex_buffer_data = new GLfloat[data.size()];

    for (unsigned int i=0; i < data.size(); i++)
    {
	packed_vertex_buffer_data[i] = data[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), packed_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete[] packed_vertex_buffer_data;
}

void SceneObject_AssimpImport::ItlCreateVerticesArray(std::vector<GLfloat> &data)
{
    unsigned int nNumVertices = 0;

    for (unsigned int i=0; i < m_nNumMeshes; i++)
    {
	nNumVertices += m_vMeshData[i]->nNumVertices;
    }

    m_pfVertices = new float[nNumVertices*3];
    unsigned int iter=0;

    for (unsigned int i=0; i < m_nNumMeshes; i++)
    {
	unsigned int nOffset = m_vMeshData[i]->nVboOffsetVertices;

        for (unsigned int ii=0; ii < m_vMeshData[i]->nNumVertices; ii++)
	{
	    m_pfVertices[iter] = data[nOffset + ii * 3];
	    m_pfVertices[iter + 1] = data[nOffset + ii * 3 + 1];
	    m_pfVertices[iter + 2] = data[nOffset + ii * 3 + 2];

	    iter += 3;
	}
    }

    assert (iter == nNumVertices*3);

    m_iNumVertices = (int) nNumVertices;

    /*m_pfVertices = new float[12];

    m_pfVertices[0] = -100.0f;
    m_pfVertices[1] = 0.0f;
    m_pfVertices[2] = -100.0f;

    m_pfVertices[3] = 100.0f;
    m_pfVertices[4] = 0.0f;
    m_pfVertices[5] = -100.0f;

    m_pfVertices[6] = 100.0f;
    m_pfVertices[7] = 0.0f;
    m_pfVertices[8] = 100.0f;

    m_pfVertices[9] = -100.0f;
    m_pfVertices[10] = 0.0f;
    m_pfVertices[11] = 100.0f;

    m_iNumVertices = 4;*/
}

void SceneObject_AssimpImport::ItlCreateIndicesArray(std::vector<GLuint> &data)
{
    unsigned int nNumIndices = 0;

    for (unsigned int i=0; i < m_nNumMeshes; i++)
    {
	nNumIndices += m_vMeshData[i]->nNumFaces * 3;
    }

    m_piIndices = new int[nNumIndices];

    unsigned int iter=0;
    unsigned int nVerticesLeftOfUs = 0;

    for (unsigned int i=0; i < m_nNumMeshes; i++)
    {
	unsigned int nOffset = m_vMeshData[i]->nIboOffsetFaces;

        for (unsigned int ii=0; ii < m_vMeshData[i]->nNumFaces*3; ii++)
	{
	    m_piIndices[iter] = data[nOffset + ii] + nVerticesLeftOfUs;
	    iter++;
	}

	nVerticesLeftOfUs += m_vMeshData[i]->nNumVertices;
    }

    assert (iter == nNumIndices);

    m_iNumIndices = (int) nNumIndices;

    /*m_piIndices = new int[6];

    m_piIndices[0] = 0;
    m_piIndices[1] = 1;
    m_piIndices[2] = 2;

    m_piIndices[3] = 1;
    m_piIndices[4] = 2;
    m_piIndices[5] = 3;

    m_iNumIndices = 6;*/
}

void SceneObject_AssimpImport::ItlCreateIndexBufferObject(std::vector<GLuint> &data)
{
    // Vertexbuffer, Indexbuffer, Normalbuffer, texcoordbuffer generieren
    glGenBuffers(1, &m_nIBO);

    //create one big array and copy vector contents
    GLuint *packed_index_buffer_data = new GLuint[data.size()];

    for (unsigned int i=0; i < data.size(); i++)
    {
	packed_index_buffer_data[i] = data[i];
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), packed_index_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] packed_index_buffer_data;
}

void SceneObject_AssimpImport::ItlPreRender()
{
    ShaderManager::instance()->PushActiveShader();
    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_SHADOWMAP)
	ShaderManager::instance()->ActivateShader("simple_shading");
    /*else if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1)
	ShaderManager::instance()->ActivateShader("assimp_deep_step1_shader");
    else if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2)
	ShaderManager::instance()->ActivateShader("assimp_deep_step2_shader");*/
    else
	ShaderManager::instance()->ActivateShader("sceneobject-assimpimport");
}

void SceneObject_AssimpImport::ItlRender()
{
    glBindVertexArray(m_nVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBO);

    const GLint l_in_Position(ShaderManager::instance()->GetAttribute("in_Position"));
    const GLint l_tangents(ShaderManager::instance()->GetAttribute("in_Tangent"));
    const GLint l_bitangents(ShaderManager::instance()->GetAttribute("in_Bitangent"));
    const GLint l_normals(ShaderManager::instance()->GetAttribute("in_Normal"));
    const GLint l_uvcoords(ShaderManager::instance()->GetAttribute("in_Texcoord"));

    GLint l_shadow_texture = ShaderManager::instance()->GetUniform("shadowmap");

    if (l_shadow_texture != -1)
    {
	    GLuint texture_unit_shadowmap = TextureManager::instance()->UseTexture("fbo_light_color");	    //load texture in texture unit
	    glUniform1i(l_shadow_texture, texture_unit_shadowmap);   //tell the shader which texture unit was used
    }


 /*   GLint l_opacity_depth_texture = ShaderManager::instance()->GetUniform("opacity_depth");
    if (l_opacity_depth_texture != -1)
    {
	    GLuint texture_unit_opacity_depth = TextureManager::instance()->UseTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit
	    glUniform1i(l_opacity_depth_texture, texture_unit_opacity_depth);   //tell the shader which texture unit was used
    }

    GLint l_opacity_opacity_texture = ShaderManager::instance()->GetUniform("opacity_opacity");
    if (l_opacity_opacity_texture != -1)
    {
	    GLuint texture_unit_opacity_opacity = TextureManager::instance()->UseTexture("fbo_deep_opacity_map_opacities");	    //load texture in texture unit
	    glUniform1i(l_opacity_opacity_texture, texture_unit_opacity_opacity);   //tell the shader which texture unit was used
    }*/

    for (unsigned int nMesh=0; nMesh < m_nNumMeshes; nMesh++)
    {
        const TItlMeshData *pMeshData = m_vMeshData[nMesh];
	const TItlMaterialData *pMaterialData = m_vMaterialData[pMeshData->nMaterialIndex];

	glUniform4fv(ShaderManager::instance()->GetUniform("MaterialAmbient"), 1, &pMaterialData->fColorAmbient[0]);
	glUniform4fv(ShaderManager::instance()->GetUniform("MaterialDiffuse"), 1, &pMaterialData->fColorDiffuse[0]);
	glUniform4fv(ShaderManager::instance()->GetUniform("MaterialSpecular"), 1, &pMaterialData->fColorSpecular[0]);
	glUniform1f(ShaderManager::instance()->GetUniform("MaterialShininess"), pMaterialData->fShininess);
	glUniform1f(ShaderManager::instance()->GetUniform("MaterialShininessStrength"), pMaterialData->fShininess_Strength);

	assert (pMaterialData->bHasColorTexture);

	if (pMaterialData->bHasColorTexture)
	{
	    glUniform1i(ShaderManager::instance()->GetUniform("color_texture"), TextureManager::instance()->UseTexture(pMaterialData->sInternalLoadedColorTexture));
	    glUniform1i(ShaderManager::instance()->GetUniform("ColorTextureAvailable"), 1);
	}
	else
	    glUniform1i(ShaderManager::instance()->GetUniform("ColorTextureAvailable"), 0);

	if (pMaterialData->bHasNormalTexture)
	{
	    glUniform1i(ShaderManager::instance()->GetUniform("normal_texture"), TextureManager::instance()->UseTexture(pMaterialData->sInternalLoadedNormalTexture));
	    glUniform1i(ShaderManager::instance()->GetUniform("NormalTextureAvailable"), 1);
	}
	else
	    glUniform1i(ShaderManager::instance()->GetUniform("NormalTextureAvailable"), 0);

	assert (l_in_Position != -1);


	if (l_in_Position != -1)
	{
	    glVertexAttribPointer(l_in_Position, 3, GL_FLOAT, GL_FALSE, 0,(const GLvoid *)(pMeshData->nVboOffsetVertices * sizeof(GLfloat)));
	    glEnableVertexAttribArray(l_in_Position);
	}

	if (l_tangents != -1)
	{
	    glVertexAttribPointer(l_tangents, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(pMeshData->nVboOffsetTangents * sizeof(GLfloat)));
	    glEnableVertexAttribArray(l_tangents);
	}

	if (l_bitangents != -1)
	{
	    glVertexAttribPointer(l_bitangents, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(pMeshData->nVboOffsetBitangents * sizeof(GLfloat)));
	    glEnableVertexAttribArray(l_bitangents);
	}

	if (l_uvcoords != -1)
	{
	    glVertexAttribPointer(l_uvcoords, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(pMeshData->nVboOffsetUVCoords * sizeof(GLfloat)));
	    glEnableVertexAttribArray(l_uvcoords);
	}

	if (l_normals != -1)
	{
	    glVertexAttribPointer(l_normals, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(pMeshData->nVboOffsetNormals * sizeof(GLfloat)));
	    glEnableVertexAttribArray(l_normals);
	}

	GLint l_use_shadow = ShaderManager::instance()->GetUniform("bUseShadow");

	if (l_use_shadow != -1)
	{
            bool bUseShadow=true;

            //Settings::instance()->GetGroup("shadow")->GetValueOrDefault("bUseShadow", false, bUseShadow);
	    glUniform1i(l_use_shadow, (int)bUseShadow);   //tell the shader which texture unit was used
	}

	glDisable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);
	glDrawElements(GL_TRIANGLES, pMeshData->nNumFaces * 3, GL_UNSIGNED_INT, (const GLvoid *) (pMeshData->nIboOffsetFaces * sizeof(GLuint)));
	//glDrawArrays(GL_TRIANGLES, pMeshData->nIboOffsetFaces, pMeshData->nNumFaces * 3);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glDrawArrays(GL_TRIANGLES, 0, pMeshData->nNumFaces * 3);

	if (pMaterialData->bHasColorTexture)
	    TextureManager::instance()->UnuseTexture(pMaterialData->sInternalLoadedColorTexture);

	if (pMaterialData->bHasNormalTexture)
	    TextureManager::instance()->UnuseTexture(pMaterialData->sInternalLoadedNormalTexture);

#ifdef CHECK_FOR_GLERROR
	GLenum error = glGetError();
	assert (error == GL_NO_ERROR);
#endif

	//Logger::debug() << pMeshData->nNumFaces << ":" << (pMeshData->nIboOffsetFaces * sizeof(GLint)) << Logger::endl;
    }

    if (l_shadow_texture != -1)
	TextureManager::instance()->UnuseTexture("fbo_light_color");	    //load texture in texture unit

    /*if (l_opacity_depth_texture != -1)
	TextureManager::instance()->UnuseTexture("fbo_deep_opacity_map_depth");	    //load texture in texture unit

    if (l_opacity_opacity_texture != -1)
	TextureManager::instance()->UnuseTexture("fbo_deep_opacity_map_opacities");	    //load texture in texture unit
*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SceneObject_AssimpImport::ItlPostRender()
{
    ShaderManager::instance()->PopActiveShader();
}

void SceneObject_AssimpImport::ItlPreRenderChildren()
{

}

void SceneObject_AssimpImport::ItlPostRenderChildren()
{

}

float *SceneObject_AssimpImport::GetVertices()
{
    assert (m_pfVertices != NULL);

    return m_pfVertices;
}


int *SceneObject_AssimpImport::GetIndices()
{
    assert (m_piIndices != NULL);

    return m_piIndices;
}

int SceneObject_AssimpImport::NumIndices()
{
    return m_iNumIndices;
}

int SceneObject_AssimpImport::NumVertices()
{
    return m_iNumVertices;
}

bool SceneObject_AssimpImport::ItlTestSkipRendering()
{
    /*if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP1)
	return true;

    if (m_pCurrentRenderInfo->tCurrentRenderPass == SceneObject_RenderPass::RENDERPASS_DEEP_OPACITY_MAP2)
	return true;*/

    return false;
}

void SceneObject_AssimpImport::ItlLoadGeneralRessources()
{
    if (s_bGeneralRessourcesInitialized == false)
    {
	// load shaders
	ShaderManager::instance()->AddShader("sceneobject-assimpimport", new Shader("shaders/sceneobject-assimpimport.vs", "shaders/sceneobject-assimpimport.fs"));
        ShaderManager::instance()->AddShader("simple_shading", new Shader("shaders/simple-shading.vs", "shaders/simple-shading.fs"));

	s_bGeneralRessourcesInitialized = true;
    }
}
