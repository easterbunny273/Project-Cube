#include "Graphics/RenderNodes/RenderNode_RenderPass.h"
#include <iostream>

Graphic::RN_RenderPass::RN_RenderPass(TRenderPass pass)
    : m_tRenderPass(pass)
{
    //nothing todo, only initializer list
}

Graphic::RN_RenderPass::TRenderPass Graphic::RN_RenderPass::getRenderState()
{
    return m_tRenderPass;
}

Graphic::RN_RenderPass::~RN_RenderPass()
{

}

void Graphic::RN_RenderPass::Render(std::shared_ptr<TItlRenderInfo> pRenderInfo)
{
    //first, save current render state
    TRenderPass old_state =  static_cast<TRenderPass>(pRenderInfo->tCurrentRenderPass);

    //add (bitwise OR) renderstate of this object to render info
    pRenderInfo->tCurrentRenderPass = m_tRenderPass;

    //render children
    Graphic::IRenderNode::Render(pRenderInfo);

    //restore old renderstate
    pRenderInfo->tCurrentRenderPass = old_state;
}
