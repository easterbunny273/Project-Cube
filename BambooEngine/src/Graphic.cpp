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

#include "GLUtils.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "BambooLib/include/Logger.h"
#include "IRenderNode.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_PostEffect.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_RenderPass.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Camera.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_FBO.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_CubeMap.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Deferred.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "Graphic.h"
#include "Camera.h"


using namespace std;

#define PI 3.14159265f

namespace BambooGraphics
{
    // initialize static members
    GraphicsCore * GraphicsCore::s_pInstance = NULL;

    /****************************************************************
      *************************************************************** */
    GraphicsCore::GraphicsCore()
    {
        m_pShaderManager = new ShaderManager();
        m_pTextureManager = new TextureManager();

        s_pInstance = this;
    }

    /****************************************************************
      *************************************************************** */
    GraphicsCore::~GraphicsCore()
    {
        assert (m_pShaderManager != NULL);
        if (m_pShaderManager != NULL)
        {
            delete m_pShaderManager;
            m_pShaderManager = NULL;
        }

        assert(m_pTextureManager != NULL);
        if (m_pTextureManager != NULL)
        {
            delete m_pTextureManager;
            m_pTextureManager = NULL;
        }
    }

    /****************************************************************
      *************************************************************** */
    void GraphicsCore::Render()
    {
        for (auto iter=m_mRenderLoops.begin(); iter != m_mRenderLoops.end(); iter++)
        {
            // get raw ptr for easier access
            TItlRenderLoop *pRenderLoop = &(iter->second);

            // clear buffers
            pRenderLoop->spRenderTarget->ClearBuffers();

            // translate semantic scene graph into a rendering scene graph
            pRenderLoop->spTranslator->Translate(pRenderLoop->pSceneRoot);

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
    ShaderManager * GraphicsCore::GetShaderManager()
    {
        assert (m_pShaderManager != NULL);

        return m_pShaderManager;
    }

    /****************************************************************
      *************************************************************** */
    TextureManager * GraphicsCore::GetTextureManager()
    {
        assert (m_pTextureManager != NULL);

        return m_pTextureManager;
    }

    /****************************************************************
      *************************************************************** */
    int GraphicsCore::AddRenderLoop(std::shared_ptr<IRenderTarget> spRenderTarget,
                              ISemanticSceneNode * pRootNode,
                              std::shared_ptr<INodeTranslator> spTranslator)
    {
        static int iID = 0;

        TItlRenderLoop NewLoop;

        NewLoop.spRenderTarget  = spRenderTarget;
        NewLoop.pSceneRoot     = pRootNode;
        NewLoop.spTranslator    = spTranslator;

        m_mRenderLoops[iID++] = NewLoop;

        // load shader, if not loaded
        GetShaderManager()->CreateAndRegisterShader("posteffect1", "BambooEngine/shaders/posteffect1.vs", "BambooEngine/shaders/posteffect1.fs");
        GetShaderManager()->CreateAndRegisterShader("directwrite", "BambooEngine/shaders/directwrite.vs", "BambooEngine/shaders/directwrite.fs");
        GetShaderManager()->CreateAndRegisterShader("light-pass", "BambooEngine/shaders/light_pass.vs", "BambooEngine/shaders/light_pass.fs");
        GetShaderManager()->CreateAndRegisterShader("camera-debug2", "BambooEngine/shaders/camera-debug2.vs", "BambooEngine/shaders/camera-debug2.fs");

       /* GetShaderManager()->AddShader("posteffect1", new ShaderManager::TShader("BambooEngine/shaders/posteffect1.vs", "BambooEngine/shaders/posteffect1.fs"));
        GetShaderManager()->AddShader("directwrite", new ShaderManager::TShader("BambooEngine/shaders/directwrite.vs", "BambooEngine/shaders/directwrite.fs"));
        GetShaderManager()->AddShader("light-pass", new ShaderManager::TShader("BambooEngine/shaders/light_pass.vs", "BambooEngine/shaders/light_pass.fs"));
        GetShaderManager()->AddShader("camera-debug2", new ShaderManager::TShader("BambooEngine/shaders/camera-debug2.vs", "BambooEngine/shaders/camera-debug2.fs"));*/
        GetTextureManager()->LoadTexture("spotlight", "textures/spot.png", false);

        return iID;
    }


    /****************************************************************
      *************************************************************** */
    void GraphicsCore::RemoveRenderLoop(int iLoopID)
    {
        assert (m_mRenderLoops.find(iLoopID) != m_mRenderLoops.end());

        m_mRenderLoops.erase(iLoopID);
    }

}
