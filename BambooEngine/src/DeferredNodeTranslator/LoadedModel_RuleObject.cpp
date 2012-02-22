#include "DeferredNodeTranslator/LoadedModel_RuleObject.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::LoadedModel_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts LoadedModel_SemSceneNode
  vAcceptedIDs.push_back(LoadedModel_SemSceneNode::ClassID());

  return vAcceptedIDs;
}
