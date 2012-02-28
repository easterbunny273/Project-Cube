#include <glm/gtc/matrix_transform.hpp>

//general includes
#include "PC_Logger.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "common_gl.h"
#include <IL/il.h>

//class specific
#include "RenderNodes/RenderNode_Generic.h"

GeometryData::TextureType tTextureTypes[4] = { GeometryData::TextureNames::ALBEDO,
                                               GeometryData::TextureNames::NORMAL,
                                               GeometryData::TextureNames::SPECULAR,
                                               GeometryData::TextureNames::DISPLACE };

Bamboo::RN_Generic::RN_Generic(std::shared_ptr<GeometryData::GenericObject> spObject)
    : m_pnVertexArrayObjects(NULL),
      m_pnVertexBufferObjects(NULL),
      m_pnIndexBufferObjects(NULL),
      m_pvsAttributeNames(NULL),
      m_pvnAttributeOffsets(NULL),
      m_pnNumIndices(NULL),
      m_pnColorTexture(NULL),
      m_pnNormalTexture(NULL),
      m_pnSpecularTexture(NULL),
      m_pnDisplaceTexture(NULL),
      m_spObject(spObject),
      m_bUseEnvironmentMapping(false)
{
    ItlLoadShader();
    ItlPrepareGLBuffers();
    ItlPrepareTextures();
}

Bamboo::RN_Generic::~RN_Generic()
{
    ItlDeleteBuffers();
    ItlDeleteTextures();
}

void Bamboo::RN_Generic::SetEnvironmentMap(GLuint nTextureID)
{
  m_bUseEnvironmentMapping = true;
  m_nEnvironmentMap = nTextureID;
}

void Bamboo::RN_Generic::ItlRender()
{
    TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
    assert (pShaderManager != NULL);

    GLuint *pTextureArrayToUse[4] = { m_pnColorTexture, m_pnNormalTexture, m_pnSpecularTexture, m_pnDisplaceTexture };

    std::vector<GLuint> vUsedTextures;
    vUsedTextures.reserve(4);


    GLint iLocationIsSphere = pShaderManager->GetUniform("bIsSphere");
    GLuint iTextureUnitForCubeMap = 15;//pTextureManager->RequestFreeUnit();

   // std::cout << iTextureUnitForCubeMap << std::endl;

    if (iLocationIsSphere != -1)
      {
        if (m_bUseEnvironmentMapping == false)
          glUniform1i(iLocationIsSphere, false);
        else
          {
            //std::cout << "is a sphere" << std::endl;
            // is a sphere
            GLint iLocationCubeMap = pShaderManager->GetUniform("cubemap_texture");

            if (iLocationCubeMap != -1)
              {
                glActiveTexture(GL_TEXTURE0+iTextureUnitForCubeMap);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_nEnvironmentMap);
                glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                glUniform1i(iLocationCubeMap, iTextureUnitForCubeMap);
                glUniform1i(iLocationIsSphere, true);
              }

          }
      }


    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        for (int i = 0; i < 4; i++)
        {
            // if we have a specific texture type
            if (m_vmTextureInformations[nMesh].find(tTextureTypes[i]) != m_vmTextureInformations[nMesh].end())
            {
                // look if shader wants the texture
                GLint iLocation = pShaderManager->GetUniform(tTextureTypes[i]);

                if (iLocation != -1)
                {
                    GLuint nTextureID = pTextureArrayToUse[i][nMesh];
                    GLuint iUsedUnit = pTextureManager->UseTexture(nTextureID);

                    glUniform1i(iLocation, iUsedUnit);

                    vUsedTextures.push_back(nTextureID);
                }
            }


        }

        glBindVertexArray(m_pnVertexArrayObjects[nMesh]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pnVertexBufferObjects[nMesh]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pnIndexBufferObjects[nMesh]);

        glDrawElements(GL_TRIANGLES, m_pnNumIndices[nMesh], GL_UNSIGNED_INT, (const GLvoid *) 0 );

        for (unsigned int i=0; i < vUsedTextures.size(); i++)
            pTextureManager->UnuseTexture(vUsedTextures[i]);

        //pTextureManager->ReleaseUnit(iTextureUnitForCubeMap);
        vUsedTextures.clear();
    }
}


void Bamboo::RN_Generic::ItlPreRender()
{

}

void Bamboo::RN_Generic::ItlPostRender()
{

}

void Bamboo::RN_Generic::ItlPreRenderChildren()
{

}

void Bamboo::RN_Generic::ItlPostRenderChildren()
{

}

bool Bamboo::RN_Generic::ItlTestSkipRendering()
{

    return false;
}

void Bamboo::RN_Generic::ItlPrepareGLBuffers()
{
    // delete old buffers, if not NULL
    ItlDeleteBuffers();

    // alloc memory for arrays
    assert (m_pnVertexBufferObjects == NULL);
    assert (m_pnVertexArrayObjects == NULL);
    assert (m_pnIndexBufferObjects == NULL);
    assert (m_pnNumIndices == NULL);
    assert (m_pvsAttributeNames == NULL);
    assert (m_pvnAttributeOffsets == NULL);

    m_nNumMeshes = m_spObject->NumMeshes();

    m_pnVertexArrayObjects = new GLuint[m_nNumMeshes];
    m_pnVertexBufferObjects = new GLuint[m_nNumMeshes];
    m_pnIndexBufferObjects = new GLuint[m_nNumMeshes];
    m_pvsAttributeNames = new std::vector<std::string>[m_nNumMeshes];
    m_pvnAttributeOffsets = new std::vector<unsigned int>[m_nNumMeshes];
    m_pnNumIndices = new unsigned int [m_nNumMeshes];
    m_vmTextureInformations.resize(m_nNumMeshes);

    // generate/allocate opengl ids for buffers

    glGenBuffers(m_nNumMeshes, m_pnVertexBufferObjects);
    glGenBuffers(m_nNumMeshes, m_pnIndexBufferObjects);


    // now, fill the buffers

    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        std::shared_ptr<GeometryData::GenericMesh> spMesh(m_spObject->GetMesh(nMesh));
        assert (spMesh);

       // GeometryData::GenericMesh *spMesh;

        std::vector<GLfloat> vBufferData;
        std::vector<unsigned int> vOffsets;
        std::vector<std::string> vsAttributeNames;

        std::vector<std::pair<std::string, std::vector<float> > > vAttributeList = spMesh->GetAttributeList();

        for (unsigned int i=0; i < vAttributeList.size(); i++)
        {
            vOffsets.push_back(vBufferData.size());
            vsAttributeNames.push_back(vAttributeList[i].first);

            for (unsigned int j=0; j < vAttributeList[i].second.size(); j++)
            {
                vBufferData.push_back(vAttributeList[i].second[j]);
            }
        }

      //if (spMesh->GetTexturePath(GeometryData::TextureNames::CUBEMAP).empty() == false)
        //  m_bUseEnvironmentMapping = true;

        for (unsigned int i=0; i < 4; i++)
        {
            float * pfTexCoords = spMesh->GetTextureCoords(tTextureTypes[i]);

            if (pfTexCoords != NULL)
            {
                m_vmTextureInformations[nMesh][tTextureTypes[i]] = vBufferData.size();

                for (unsigned int j=0; j < spMesh->NumVertices() * 2; j++)
                {
                    vBufferData.push_back(pfTexCoords[j]);
                }
            }
        }

        m_pvsAttributeNames[nMesh] = vsAttributeNames;
        m_pvnAttributeOffsets[nMesh] = vOffsets;

        glBindBuffer(GL_ARRAY_BUFFER, m_pnVertexBufferObjects[nMesh]);
        glBufferData(GL_ARRAY_BUFFER, vBufferData.size() * sizeof(GLfloat), &(vBufferData[0]), GL_STATIC_DRAW);

        // now fill the indices buffer
        unsigned int *pIndices = spMesh->GetIndices(m_pnNumIndices[nMesh]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pnIndexBufferObjects[nMesh]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pnNumIndices[nMesh] * sizeof(GLuint), pIndices, GL_STATIC_DRAW);
    }

    ItlPrepareVAO();

}

void Bamboo::RN_Generic::ItlDeleteBuffers()
{
    if (m_pnVertexBufferObjects != NULL)
    {
        assert (m_pnIndexBufferObjects != NULL);

        glDeleteBuffers(m_nNumMeshes, m_pnVertexBufferObjects);
        glDeleteBuffers(m_nNumMeshes, m_pnIndexBufferObjects);
        glDeleteVertexArrays(m_nNumMeshes, m_pnVertexArrayObjects);

        delete[] m_pnVertexArrayObjects;
        m_pnVertexArrayObjects = NULL;

        delete[] m_pnVertexBufferObjects;
        m_pnVertexBufferObjects = NULL;

        delete[] m_pnIndexBufferObjects;
        m_pnIndexBufferObjects = NULL;

        delete [] m_pvsAttributeNames;
        m_pvsAttributeNames = NULL;

        delete [] m_pvnAttributeOffsets;
        m_pvnAttributeOffsets = NULL;
    }

}

void Bamboo::RN_Generic::ItlPrepareVAO()
{
    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
    assert (pShaderManager != NULL);

    pShaderManager->PushActiveShader();

    pShaderManager->ActivateShader("deferred_pass_cm");

    // generate arrays
    glGenVertexArrays(m_nNumMeshes, m_pnVertexArrayObjects);


    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        glBindVertexArray(m_pnVertexArrayObjects[nMesh]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pnVertexBufferObjects[nMesh]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pnIndexBufferObjects[nMesh]);

        // set attrib pointers
        for (unsigned int i=0; i < m_pvsAttributeNames[nMesh].size(); i++)
        {
            std::string sCurrentAttributeName(m_pvsAttributeNames[nMesh][i]);

            const GLint l_current_attribute (ItlGetGraphicCore()->GetShaderManager()->GetAttribute(sCurrentAttributeName));

            unsigned int nOffset = m_pvnAttributeOffsets[nMesh][i];

            if (l_current_attribute != -1)
            {
                glVertexAttribPointer(l_current_attribute, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(nOffset * sizeof(GLfloat)));
                glEnableVertexAttribArray(l_current_attribute);
            }
        }

        // set texcoords pointers
        for (auto iter = m_vmTextureInformations[nMesh].begin(); iter != m_vmTextureInformations[nMesh].end(); iter++)
        {
            std::string sCurrentAttributeName(iter->first);

            sCurrentAttributeName.append("_coords");

            Logger::debug() << sCurrentAttributeName << Logger::endl;

            const GLint l_current_attribute (ItlGetGraphicCore()->GetShaderManager()->GetAttribute(sCurrentAttributeName));

            unsigned int nOffset = iter->second;

            if (l_current_attribute != -1)
            {
                glVertexAttribPointer(l_current_attribute, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(nOffset * sizeof(GLfloat)));
                glEnableVertexAttribArray(l_current_attribute);
            }
        }
    }

    pShaderManager->PopActiveShader();
}

void Bamboo::RN_Generic::ItlLoadShader()
{
    static bool bAlreadyLoaded = false;

    if (bAlreadyLoaded == false)
    {
      ItlGetGraphicCore()->GetShaderManager()->AddShader("deferred_pass", new Shader("BambooEngine/shaders/deferred_pass.vert", "BambooEngine/shaders/deferred_pass.frag"));
      ItlGetGraphicCore()->GetShaderManager()->AddShader("deferred_pass_cm", new Shader("BambooEngine/shaders/deferred_pass_cm.vert", "BambooEngine/shaders/deferred_pass_cm.geom", "BambooEngine/shaders/deferred_pass_cm.frag"));

      bAlreadyLoaded = true;

      GLenum error = glGetError();

      // load test texture
      ILuint pnTextures[6];
      std::string sFiles[6];
      sFiles[0] = std::string("textures/cubemap_debug_left.jpg");
      sFiles[4] = std::string("textures/cubemap_debug_front.jpg");
      sFiles[1] = std::string("textures/cubemap_debug_right.jpg");
      sFiles[5] = std::string("textures/cubemap_debug_back.jpg");
      sFiles[2] = std::string("textures/cubemap_debug_top.jpg");
      sFiles[3] = std::string("textures/cubemap_debug_bottom.jpg");


      GLuint nNewTexture;

      //generate color texture (=create new opengl id)
      glGenTextures(1, &nNewTexture);

      std::cout << nNewTexture << std::endl;

      //bind color texture
      glBindTexture(GL_TEXTURE_CUBE_MAP, nNewTexture);

      error = glGetError();
      assert (error == GL_NO_ERROR);

      ilInit();
      ilGenImages(6,&pnTextures[0]);                   // generieren von IL ID für Texturen

      for (unsigned int i=0; i < 6; i++)
      {
          ilBindImage(pnTextures[i]);

          std::string sFileName = sFiles[i];
          bool bOk = ilLoadImage (sFileName.c_str());
          assert (bOk);

          long int iHeight, iWidth, iFormat;
          unsigned char *szData=0;

          iWidth=ilGetInteger(IL_IMAGE_WIDTH);		    // Breite des Bildes holen
          iHeight=ilGetInteger(IL_IMAGE_HEIGHT);	    // Höhe des Bildes holen
          //bpp=ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // Farbtiefe des Bildes
          iFormat=ilGetInteger(IL_IMAGE_FORMAT);	    // Format des Bildes z.B. RGB RGBA BGR BGRA usw.
          szData=ilGetData();			    // Zeiger auf Bilddaten holen

          assert (iWidth == iHeight);
          assert (iWidth == 512);

          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, 512, 512, 0, iFormat, GL_UNSIGNED_BYTE, szData);


      }

      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);



    }
}

void Bamboo::RN_Generic::ItlPrepareTextures()
{
    TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    assert (m_pnColorTexture == NULL);
    m_pnColorTexture    = new GLuint[m_nNumMeshes];
    m_pnNormalTexture   = new GLuint[m_nNumMeshes];
    m_pnSpecularTexture = new GLuint[m_nNumMeshes];
    m_pnDisplaceTexture = new GLuint[m_nNumMeshes];

    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        std::shared_ptr<GeometryData::GenericMesh> spMesh(m_spObject->GetMesh(nMesh));

        GLuint * psTargetArrays [4] = { &m_pnColorTexture[nMesh], &m_pnNormalTexture[nMesh], &m_pnSpecularTexture[nMesh], &m_pnDisplaceTexture[nMesh] };
        bool bGammaCorrected[4] = { true, false, false, false };

        for (int i=0; i < 4; i++)
        {
            std::string sTexturePath = spMesh->GetTexturePath(tTextureTypes[i]);

            if (sTexturePath.empty() == false)
            {
                std::string sFullTexturePath = std::string("models/") + sTexturePath;
                pTextureManager->LoadTexture(*(psTargetArrays[i]), sFullTexturePath, bGammaCorrected[i]);
            }
        }
    }


}

void Bamboo::RN_Generic::ItlDeleteTextures()
{
    TextureManager *pTextureManager = ItlGetGraphicCore()->GetTextureManager();
    assert (pTextureManager != NULL);

    assert (m_pnColorTexture != NULL);
    delete[] m_pnColorTexture;
    m_pnColorTexture = NULL;

    assert (m_pnNormalTexture != NULL);
    delete[] m_pnNormalTexture;
    m_pnNormalTexture = NULL;

    assert (m_pnSpecularTexture != NULL);
    delete[] m_pnSpecularTexture;
    m_pnSpecularTexture = NULL;

    assert (m_pnDisplaceTexture != NULL);
    delete[] m_pnDisplaceTexture;
    m_pnDisplaceTexture = NULL;
}
