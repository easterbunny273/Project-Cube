#include "DeferredNodeTranslator/Light_RuleObject.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Light_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Light_SemSceneNode
  vAcceptedIDs.push_back(Light_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Light_RuleObject::Action()
{

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Light_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  Light_RuleObject *pNewObject = new Light_RuleObject();

  pNewObject->m_spSemNode = spSemNode;
  pNewObject->m_pTranslator = pTranslator;

  return pNewObject;
}
