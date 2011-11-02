#include "SceneObjects/LoadedModel.h"
#include "RenderNodes/RenderNode_AssimpImport.h"

#include <memory>

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_LoadedModel::CreateRenderNode()
{
    assert (m_sFilename.size() > 0);

    std::shared_ptr<Bamboo::IRenderNode> spRenderNode(new Bamboo::RN_AssimpImport(m_sFilename));

    return spRenderNode;
}

std::shared_ptr<Bamboo::SO_LoadedModel> Bamboo::SO_LoadedModel::Create(std::string sFilename)
{
    std::shared_ptr<Bamboo::SO_LoadedModel> spNewNode(new SO_LoadedModel());

    spNewNode->m_sFilename = sFilename;

    return spNewNode;
}
