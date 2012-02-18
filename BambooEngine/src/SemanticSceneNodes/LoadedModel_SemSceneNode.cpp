#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"

std::shared_ptr<LoadedModel_SemSceneNode> LoadedModel_SemSceneNode::Create(std::string sFilename)
{
  // create node
  LoadedModel_SemSceneNode *pNode = new LoadedModel_SemSceneNode();

  // set parameters
  pNode->m_sFilename = sFilename;

  // create shared_ptr
  std::shared_ptr<LoadedModel_SemSceneNode> spNode(pNode);

  return spNode;
}

LoadedModel_SemSceneNode::~LoadedModel_SemSceneNode()
{
  // nothing to do
}

ISemanticSceneNode::t_classID LoadedModel_SemSceneNode::ClassID()
{
  return 1;
}

LoadedModel_SemSceneNode::LoadedModel_SemSceneNode() : ISemanticSceneNode(ClassID())
{
  // nothing to do
}

std::string LoadedModel_SemSceneNode::GetFilename()
{
  return m_sFilename;
}
