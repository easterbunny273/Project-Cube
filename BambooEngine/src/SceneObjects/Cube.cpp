#include "SceneObjects/Cube.h"
#include "RenderNodes/RenderNode_Generic.h"

#include "Gamelogic/Cube.h"

#include <memory>

std::shared_ptr<Bamboo::SO_Cube> Bamboo::SO_Cube::Create(Cube *pCube)
{
    std::shared_ptr<Bamboo::SO_Cube> spNewNode(new SO_Cube());

    spNewNode->m_pCube = pCube;

    return spNewNode;
}

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_Cube::CreateRenderNode()
{
    std::shared_ptr<Bamboo::IRenderNode> spRenderNode(new Bamboo::RN_Generic(m_pCube->GenerateGenericObject()));

    return spRenderNode;
}
