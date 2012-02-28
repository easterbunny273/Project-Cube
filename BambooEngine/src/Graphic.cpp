/*
* source file for Graphic class
* written by: christian moellinger <ch.moellinger@gmail.com>
* 10/2010
*/

#include <string>
#include <sstream>
#include <memory>

//#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common_gl.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "PC_Logger.h"
#include "RenderNodes/IRenderNode.h"
#include "RenderNodes/RenderNode_PostEffect.h"
#include "RenderNodes/RenderNode_RenderPass.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_FBO.h"
#include "RenderNodes/RenderNode_CubeMap.h"
#include "RenderNodes/RenderNode_Deferred.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "Graphic.h"
#include "Camera.h"

#include "Events.h"
#include "MainApp.h"

using namespace std;

#define PI 3.14159265f

// initialize static members
Bamboo * Bamboo::s_pInstance = NULL;

/****************************************************************
  *************************************************************** */
Bamboo::Bamboo()
{
    m_pShaderManager = new ShaderManager();
    m_pTextureManager = new TextureManager();

    s_pInstance = this;
}

/****************************************************************
  *************************************************************** */
Bamboo::~Bamboo()
{
    assert (m_pShaderManager != NULL);
    if (m_pShaderManager != NULL)
        delete m_pShaderManager;
    m_pShaderManager = NULL;

    assert(m_pTextureManager != NULL);
    if (m_pTextureManager != NULL)
        delete m_pTextureManager;
    m_pTextureManager = NULL;
}

/****************************************************************
  *************************************************************** */
void Bamboo::Render()
{
    for (auto iter=m_mRenderLoops.begin(); iter != m_mRenderLoops.end(); iter++)
    {
        // get raw ptr for easier access
        TItlRenderLoop *pRenderLoop = &(iter->second);

        // clear buffers
        pRenderLoop->spRenderTarget->ClearBuffers();

        // translate semantic scene graph into a rendering scene graph
        pRenderLoop->spTranslator->Translate(pRenderLoop->spSceneRoot);

        // get render scene graph from translator
        std::shared_ptr<IRenderNode> spRenderNode = pRenderLoop->spTranslator->GetRenderGraph();
        assert (spRenderNode);

        // start rendering
        spRenderNode->Render();

        // swap buffers
        pRenderLoop->spRenderTarget->SwapBuffers();
    }
}


/****************************************************************
  *************************************************************** */
Bamboo::ShaderManager * Bamboo::GetShaderManager()
{
    assert (m_pShaderManager != NULL);

    return m_pShaderManager;
}

/****************************************************************
  *************************************************************** */
Bamboo::TextureManager * Bamboo::GetTextureManager()
{
    assert (m_pTextureManager != NULL);

    return m_pTextureManager;
}

/****************************************************************
  *************************************************************** */
int Bamboo::AddRenderLoop(std::shared_ptr<IRenderTarget> spRenderTarget,
                          std::shared_ptr<ISemanticSceneNode> spRootNode,
                          std::shared_ptr<INodeTranslator> spTranslator)
{
    static int iID = 0;

    TItlRenderLoop NewLoop;

    NewLoop.spRenderTarget  = spRenderTarget;
    NewLoop.spSceneRoot     = spRootNode;
    NewLoop.spTranslator    = spTranslator;

    m_mRenderLoops[iID++] = NewLoop;

    // load shader, if not loaded
    GetShaderManager()->AddShader("posteffect1", new Bamboo::Shader("BambooEngine/shaders/posteffect1.vs", "BambooEngine/shaders/posteffect1.fs"));
    GetShaderManager()->AddShader("directwrite", new Bamboo::Shader("BambooEngine/shaders/directwrite.vs", "BambooEngine/shaders/directwrite.fs"));
    GetShaderManager()->AddShader("light-pass", new Bamboo::Shader("BambooEngine/shaders/light_pass.vs", "BambooEngine/shaders/light_pass.fs"));
    GetShaderManager()->AddShader("camera-debug2", new Bamboo::Shader("BambooEngine/shaders/camera-debug2.vs", "BambooEngine/shaders/camera-debug2.fs"));
    GetTextureManager()->LoadTexture("spotlight", "textures/spot.png", false);

    return iID;
}


/****************************************************************
  *************************************************************** */
void Bamboo::RemoveRenderLoop(int iLoopID)
{
    assert (m_mRenderLoops.find(iLoopID) != m_mRenderLoops.end());

    m_mRenderLoops.erase(iLoopID);
}
