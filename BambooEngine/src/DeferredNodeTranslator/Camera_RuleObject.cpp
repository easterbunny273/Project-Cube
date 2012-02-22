#include "DeferredNodeTranslator/Camera_RuleObject.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Camera_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Camera_SemSceneNode
  vAcceptedIDs.push_back(Camera_SemSceneNode::ClassID());

  return vAcceptedIDs;
}
