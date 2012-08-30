#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"

// register class at core system and get unique class id
BambooLib::t_classID LoadedModel_SemSceneNode::s_ClassID = BambooLib::CoreSystem::GetInstance()->RegisterClass("SM_LOADM", NULL);

std::shared_ptr<LoadedModel_SemSceneNode> LoadedModel_SemSceneNode::Create(std::string sFilename)
{
  // create node
  LoadedModel_SemSceneNode *pNode = new LoadedModel_SemSceneNode();

  // set parameters
  pNode->m_sFilename            = sFilename;
  pNode->m_bEnvironmentMapping  = false;
  pNode->m_mTransformMatrix = glm::mat4();

  // create shared_ptr
  std::shared_ptr<LoadedModel_SemSceneNode> spNode(pNode);

  return spNode;
}

LoadedModel_SemSceneNode::~LoadedModel_SemSceneNode()
{
  // nothing to do
}


LoadedModel_SemSceneNode::LoadedModel_SemSceneNode() : ISemanticSceneNode(ClassID()), IIdentifyable(ClassID())
{
  // initialise variables
  m_sFilename = std::string();
  m_bEnvironmentMapping = false;
}

std::string LoadedModel_SemSceneNode::GetFilename()
{
  return m_sFilename;
}

void LoadedModel_SemSceneNode::ActivateEnvironmentMapping()
{
  m_bEnvironmentMapping       = true;
}

void LoadedModel_SemSceneNode::DeactivateEnvironmentMapping()
{
  m_bEnvironmentMapping       = false;
}
