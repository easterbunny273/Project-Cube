#include "SceneObjects/LoadedModel.h"
#include "RenderNodes/RenderNode_Generic.h"
#include "AssimpWrapper.h"

#include <memory>

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_LoadedModel::CreateRenderNode()
{
    assert (m_sFilename.size() > 0);


    std::shared_ptr<Bamboo::IRenderNode> spNewRenderNode(new Bamboo::RN_Generic(AssimpWrapper::LoadModel(m_sFilename)));
    //std::shared_ptr<Bamboo::IRenderNode> spRenderNode(new Bamboo::RN_AssimpImport(m_sFilename));

    return spNewRenderNode;
}

std::shared_ptr<Bamboo::SO_LoadedModel> Bamboo::SO_LoadedModel::Create(std::string sFilename)
{
    std::shared_ptr<Bamboo::SO_LoadedModel> spNewNode(new SO_LoadedModel());

    spNewNode->m_sFilename = sFilename;

    return spNewNode;
}
