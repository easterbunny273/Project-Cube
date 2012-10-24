#include "RenderNodes/RenderNode_RenderPass.h"
#include <iostream>

namespace BambooGraphics
{
GraphicsCore::RN_RenderPass::RN_RenderPass(TRenderPass pass)
    : m_tRenderPass(pass)
{
    //nothing todo, only initializer list
}

GraphicsCore::RN_RenderPass::TRenderPass GraphicsCore::RN_RenderPass::getRenderState()
{
    return m_tRenderPass;
}

GraphicsCore::RN_RenderPass::~RN_RenderPass()
{

}

void GraphicsCore::RN_RenderPass::Render(std::shared_ptr<TItlRenderInfo> pRenderInfo)
{
    //first, save current render state
    TRenderPass old_state =  static_cast<TRenderPass>(pRenderInfo->tCurrentRenderPass);

    //add (bitwise OR) renderstate of this object to render info
    pRenderInfo->tCurrentRenderPass = m_tRenderPass;

    //render children
    GraphicsCore::IRenderNode::Render(pRenderInfo);

    //restore old renderstate
    pRenderInfo->tCurrentRenderPass = old_state;
}

}

