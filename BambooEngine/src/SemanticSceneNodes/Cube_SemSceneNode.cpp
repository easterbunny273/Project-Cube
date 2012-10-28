#include "SemanticSceneNodes/Cube_SemSceneNode.h"

#include <cassert>

// register class at core system and get unique class id
BambooLib::t_classID Cube_SemSceneNode::s_ClassID = BambooLib::CoreSystem::GetInstance()->RegisterClass("SM_CUBE_", NULL);

Cube_SemSceneNode * Cube_SemSceneNode::Create(Cube *pCube)
{
  // create new node
  Cube_SemSceneNode * pNewNode = new Cube_SemSceneNode();
  assert (pNewNode != NULL);

  // set initial parameters
  pNewNode->m_pCube = pCube;
  pNewNode->m_mTransformMatrix = glm::translate(glm::mat4(), glm::vec3(pCube->GetCubePosition()));

  // return new node
  return pNewNode;
}


Cube_SemSceneNode::Cube_SemSceneNode() : ISemanticSceneNode(ClassID()), IIdentifyable(ClassID())
{
  // nothing to do, only initializer list
}

Cube_SemSceneNode::~Cube_SemSceneNode()
{
  // nothing to do
}
