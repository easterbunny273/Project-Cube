#include "SemanticSceneNodes/FractalTerrain_SemSceneNode.h"

std::shared_ptr<FractalTerrain_SemSceneNode> FractalTerrain_SemSceneNode::Create(std::string sFilename)
{
  // create node
  FractalTerrain_SemSceneNode *pNode = new FractalTerrain_SemSceneNode();

  // set parameters
  pNode->m_sFilename            = sFilename;

  // create shared_ptr
  std::shared_ptr<FractalTerrain_SemSceneNode> spNode(pNode);

  return spNode;
}

FractalTerrain_SemSceneNode::~FractalTerrain_SemSceneNode()
{
  // nothing to do
}

ISemanticSceneNode::t_classID FractalTerrain_SemSceneNode::ClassID()
{
  return 4;
}

FractalTerrain_SemSceneNode::FractalTerrain_SemSceneNode() : ISemanticSceneNode(ClassID())
{
  // initialize variables
  m_sFilename = std::string();
}

std::string FractalTerrain_SemSceneNode::GetFilename()
{
  return m_sFilename;
}
