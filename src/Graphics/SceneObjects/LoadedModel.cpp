#include "Graphics/SceneObjects/LoadedModel.h"
#include "Graphics/RenderNodes/RenderNode_AssimpImport.h"

#include <memory>

std::shared_ptr<Graphic::IRenderNode> Graphic::SO_LoadedModel::CreateRenderNode()
{
    assert (m_sFilename.size() > 0);

    std::shared_ptr<Graphic::IRenderNode> spRenderNode(new Graphic::RN_AssimpImport(m_sFilename));

    return spRenderNode;
}

std::shared_ptr<Graphic::SO_LoadedModel> Graphic::SO_LoadedModel::Create(std::string sFilename)
{
    std::shared_ptr<Graphic::SO_LoadedModel> spNewNode(new SO_LoadedModel());

    spNewNode->m_sFilename = sFilename;

    return spNewNode;
}
