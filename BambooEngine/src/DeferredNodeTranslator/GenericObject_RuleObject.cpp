#include "DeferredNodeTranslator/GenericObject_RuleObject.h"
#include "SemanticSceneNodes/GenericObject_SemSceneNode.h"
#include "RenderNodes/RenderNode_Generic.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"
#include "AssimpWrapper.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::GenericObject_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts LoadedModel_SemSceneNode
  vAcceptedIDs.push_back(GenericObject_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

DeferredNodeTranslator::GenericObject_RuleObject::~GenericObject_RuleObject()
{
  std::cout << "delete" << std::endl;
}

void DeferredNodeTranslator::GenericObject_RuleObject::Action()
{
  assert (m_pTranslator->m_spRootNode);
  assert (m_pTranslator->m_spDeferredNode);

  if (m_spCorrespondingRenderingNode)
    {
      if (m_bActive == false)
        {
        m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);
        m_bActive = true;
        }

    }
  else
    {
      m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_Generic>(new Bamboo::RN_Generic(m_spSemNode->GetObjectSP()));

      m_spCorrespondingRenderingNode->SetEnvironmentMapping(false);

      glm::mat4 * pmModelMatrix = (glm::mat4 *) m_spSemNode->GetObject()->GetMeshPtr(0)->GetModelMatrix();
      //todo: move into RN_GENERIC
      m_spCorrespondingRenderingNode->SetTransformMatrix(*pmModelMatrix);

      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);
      m_bActive = true;
     // m_pTranslator->m_vShadowCasterNodes.push_back(m_spCorrespondingRenderingNode);
    }

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::GenericObject_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  assert (spSemNode);

  GenericObject_RuleObject *pNewObject = new GenericObject_RuleObject();

  pNewObject->m_pTranslator = pTranslator;

  pNewObject->m_spSemNode = std::dynamic_pointer_cast<GenericObject_SemSceneNode>(spSemNode);
  pNewObject->m_spSemNode->RegisterDeleteListener(pNewObject);
  pNewObject->m_bActive = false;

  assert (pNewObject->m_spSemNode);


  return pNewObject;
}

void DeferredNodeTranslator::GenericObject_RuleObject::OnDelete(ISemanticSceneNode *pNode)
{
  assert (pNode == m_spSemNode.get());

  m_pTranslator->m_spDeferredNode->RemoveChild(m_spCorrespondingRenderingNode);

  m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_Generic>();
}

void DeferredNodeTranslator::GenericObject_RuleObject::OnRemovedFromParent(ISemanticSceneNode *pParent,
                                                                           ISemanticSceneNode *pNode)
{
 // std::cout << std::hex << pParent << " " << pNode << std::dec << std::endl;

  assert (pNode == m_spSemNode.get());

  assert (m_spCorrespondingRenderingNode);

  m_pTranslator->m_spDeferredNode->RemoveChild(m_spCorrespondingRenderingNode);
  m_bActive = false;

  //m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_Generic>();
}
