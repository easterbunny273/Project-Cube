#include "Graphics/RenderNodes/RenderNode_RenderPass.h"
#include <iostream>

RenderNode_RenderPass::RenderNode_RenderPass(TRenderPass pass)
    : m_tRenderPass(pass)
{
    //nothing todo, only initializer list
}

RenderNode_RenderPass::TRenderPass RenderNode_RenderPass::getRenderState()
{
    return m_tRenderPass;
}

RenderNode_RenderPass::~RenderNode_RenderPass()
{

}

void RenderNode_RenderPass::Render(std::shared_ptr<TItlRenderInfo> pRenderInfo)
{
    //first, save current render state
    TRenderPass old_state =  static_cast<TRenderPass>(pRenderInfo->tCurrentRenderPass);

    //add (bitwise OR) renderstate of this object to render info
    pRenderInfo->tCurrentRenderPass = m_tRenderPass;

    //render children
    RenderNode::Render(pRenderInfo);

    //restore old renderstate
    pRenderInfo->tCurrentRenderPass = old_state;
}
