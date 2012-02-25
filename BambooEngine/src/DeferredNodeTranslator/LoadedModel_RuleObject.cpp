#include "DeferredNodeTranslator/LoadedModel_RuleObject.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
#include "RenderNodes/RenderNode_Generic.h"
#include "AssimpWrapper.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::LoadedModel_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts LoadedModel_SemSceneNode
  vAcceptedIDs.push_back(LoadedModel_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::LoadedModel_RuleObject::Action()
{
  assert (m_pTranslator->m_spRootNode);
  assert (m_pTranslator->m_spDeferredNode);

  if (m_spCorrespondingRenderingNode)
    {
      // only update values
    }
  else
    {
      // create new node
      m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::IRenderNode>(new Bamboo::RN_Generic(AssimpWrapper::LoadModel(m_spSemNode->GetFilename())));
      assert (m_spCorrespondingRenderingNode);

      // attach to deferred node
      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);
    }

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::LoadedModel_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  LoadedModel_RuleObject *pNewObject = new LoadedModel_RuleObject();

  pNewObject->m_pTranslator = pTranslator;

  pNewObject->m_spSemNode = std::dynamic_pointer_cast<LoadedModel_SemSceneNode>(spSemNode);
  assert (pNewObject->m_spSemNode);


  return pNewObject;
}
