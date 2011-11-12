//general includes
#include "Logger.h"
#include "ShaderManager.h"
#include "TextureManager.h"

//class specific
#include "RenderNodes/RenderNode_Generic.h"


Bamboo::RN_Generic::RN_Generic(std::shared_ptr<GeometryData::GenericObject> spObject)
    : m_pnVertexArrayObjects(NULL),
      m_pnVertexBufferObjects(NULL),
      m_pnIndexBufferObjects(NULL),
      m_pvsAttributeNames(NULL),
      m_pvnAttributeOffsets(NULL),
      m_pnNumIndices(NULL),
      m_spObject(spObject)
{
    ItlLoadShader();
    ItlPrepareGLBuffers();
}

Bamboo::RN_Generic::~RN_Generic()
{
    ItlDeleteBuffers();
}

void Bamboo::RN_Generic::ItlRender()
{
    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        glBindVertexArray(m_pnVertexArrayObjects[nMesh]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pnVertexBufferObjects[nMesh]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pnIndexBufferObjects[nMesh]);

        glDrawElements(GL_TRIANGLES, m_pnNumIndices[nMesh], GL_UNSIGNED_INT, (const GLvoid *) 0 );
    }
}


void Bamboo::RN_Generic::ItlPreRender()
{
    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
    assert (pShaderManager != NULL);

    pShaderManager->PushActiveShader();
    pShaderManager->ActivateShader("GenericShader1");
}

void Bamboo::RN_Generic::ItlPostRender()
{
    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
    assert (pShaderManager != NULL);

    pShaderManager->PopActiveShader();
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
    // first, create the buffers

    // delete old buffers, if not NULL


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

    // generate/allocate opengl ids for buffers

    glGenBuffers(m_nNumMeshes, m_pnVertexBufferObjects);
    glGenBuffers(m_nNumMeshes, m_pnIndexBufferObjects);


    // now, fill the buffers

    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        std::shared_ptr<GeometryData::GenericMesh> spMesh(m_spObject->GetMesh(nMesh));
        assert (spMesh);

        //GeometryData::GenericMesh *spMesh;

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
    pShaderManager->ActivateShader("GenericShader1");

    // generate arrays
    glGenVertexArrays(m_nNumMeshes, m_pnVertexArrayObjects);


    for (unsigned int nMesh = 0; nMesh < m_nNumMeshes; nMesh++)
    {
        glBindVertexArray(m_pnVertexArrayObjects[nMesh]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pnVertexBufferObjects[nMesh]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pnIndexBufferObjects[nMesh]);

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
    }

    pShaderManager->PopActiveShader();
}

void Bamboo::RN_Generic::ItlLoadShader()
{
    ShaderManager *pShaderManager = ItlGetGraphicCore()->GetShaderManager();
    assert (pShaderManager != NULL);

    pShaderManager->AddShader("GenericShader1", new Shader("BambooEngine/shaders/generic1.vs", "BambooEngine/shaders/generic1.fs"));
}
