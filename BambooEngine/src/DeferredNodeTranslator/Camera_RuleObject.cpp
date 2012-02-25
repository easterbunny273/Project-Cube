#include "DeferredNodeTranslator/Camera_RuleObject.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Camera_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Camera_SemSceneNode
  vAcceptedIDs.push_back(Camera_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Camera_RuleObject::Action()
{

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Camera_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode)
{
  Camera_RuleObject *pNewObject = new Camera_RuleObject();

  pNewObject->m_spSemNode = spSemNode;

  return pNewObject;
}
