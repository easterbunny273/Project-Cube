#include "SemanticSceneNodes/Cube_SemSceneNode.h"

#include <cassert>

std::shared_ptr<Cube_SemSceneNode> Cube_SemSceneNode::Create(Cube *pCube)
{
  // create new node
  Cube_SemSceneNode * pNewNode = new Cube_SemSceneNode();
  assert (pNewNode != NULL);

  // set initial parameters
  pNewNode->m_pCube = pCube;

  // create shared_ptr
  std::shared_ptr<Cube_SemSceneNode> spNewNode(pNewNode);

  // return shared ptr
  return spNewNode;
}

ISemanticSceneNode::t_classID Cube_SemSceneNode::ClassID()
{
  return 3;
}


Cube_SemSceneNode::Cube_SemSceneNode() : ISemanticSceneNode(ClassID())
{
  // nothing to do, only initializer list
}

Cube_SemSceneNode::~Cube_SemSceneNode()
{
  // nothing to do
}
