#include "Graphics/SceneObjects/Cube.h"
#include "Graphics/RenderNodes/RenderNode_Cube.h"

#include <memory>

std::shared_ptr<Bamboo::SO_Cube> Bamboo::SO_Cube::Create()
{
    std::shared_ptr<Bamboo::SO_Cube> spNewNode(new SO_Cube());

    return spNewNode;
}

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_Cube::CreateRenderNode()
{
    std::shared_ptr<Bamboo::IRenderNode> spRenderNode(new Bamboo::RN_Cube());

    return spRenderNode;
}
