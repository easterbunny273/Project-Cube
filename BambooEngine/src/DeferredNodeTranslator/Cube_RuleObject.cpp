#include "DeferredNodeTranslator/Cube_RuleObject.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Cube_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Cube_SemSceneNode
  vAcceptedIDs.push_back(Cube_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Cube_RuleObject::Action()
{

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Cube_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  Cube_RuleObject *pNewObject = new Cube_RuleObject();

  pNewObject->m_pTranslator   = pTranslator;
  pNewObject->m_spSemNode     = spSemNode;

  return pNewObject;
}
