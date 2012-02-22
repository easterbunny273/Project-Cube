#include "DeferredNodeTranslator/Light_RuleObject.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Light_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Light_SemSceneNode
  vAcceptedIDs.push_back(Light_SemSceneNode::ClassID());

  return vAcceptedIDs;
}
