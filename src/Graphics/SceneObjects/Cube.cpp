#include "Graphics/SceneObjects/Cube.h"
#include "Graphics/RenderNodes/RenderNode_Cube.h"

#include <memory>

std::shared_ptr<Graphic::SO_Cube> Graphic::SO_Cube::Create()
{
    std::shared_ptr<Graphic::SO_Cube> spNewNode(new SO_Cube());

    return spNewNode;
}

std::shared_ptr<Graphic::IRenderNode> Graphic::SO_Cube::CreateRenderNode()
{
    std::shared_ptr<Graphic::IRenderNode> spRenderNode(new Graphic::RN_Cube());

    return spRenderNode;
}
