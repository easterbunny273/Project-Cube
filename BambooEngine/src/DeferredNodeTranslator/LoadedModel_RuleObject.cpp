#include "DeferredNodeTranslator/LoadedModel_RuleObject.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::LoadedModel_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts LoadedModel_SemSceneNode
  vAcceptedIDs.push_back(LoadedModel_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::LoadedModel_RuleObject::Action()
{

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::LoadedModel_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode)
{
  LoadedModel_RuleObject *pNewObject = new LoadedModel_RuleObject();

  pNewObject->m_spSemNode = spSemNode;

  return pNewObject;
}
