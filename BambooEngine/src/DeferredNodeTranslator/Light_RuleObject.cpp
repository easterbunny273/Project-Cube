#include "DeferredNodeTranslator/Light_RuleObject.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"
#include "RenderNodes/RenderNode_SpotLight.h"
#include "RenderNodes/RenderNode_SpotLight_Model.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Light_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Light_SemSceneNode
  vAcceptedIDs.push_back(Light_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Light_RuleObject::Action()
{
  float fFOV, fNear, fFar;
  glm::vec3 vColor, vPosition, vLookAt;

  m_spSemNode->GetLightParameters(vPosition, vLookAt, fFOV, vColor, fNear, fFar);

  if (m_spCorrespondingRenderingNode)
    {
      // only update parameters
      m_spCorrespondingRenderingNode->SetTransformMatrix(m_spSemNode->GetTransformMatrix());
      m_spCorrespondingRenderingNode_Model->SetTransformMatrix(m_spSemNode->GetTransformMatrix());

      m_spCorrespondingRenderingNode->ClearChilds();
      for (unsigned int i=0; i < m_pTranslator->m_vShadowCasterNodes.size(); i++)
        m_spCorrespondingRenderingNode->AddChild(m_pTranslator->m_vShadowCasterNodes.at(i));
    }
  else
    {
      m_spCorrespondingRenderingNode = std::shared_ptr<Bamboo::RN_SpotLight>(new Bamboo::RN_SpotLight(vPosition, vLookAt, fFOV, vColor));
      assert (m_spCorrespondingRenderingNode);

      m_pTranslator->m_vLightNodes.push_back(m_spCorrespondingRenderingNode);

      m_spCorrespondingRenderingNode->SetGraphicCore(m_pTranslator->m_pCore);

      m_spCorrespondingRenderingNode_Model = std::shared_ptr<Bamboo::RN_SpotLight_Model>(new Bamboo::RN_SpotLight_Model(vPosition, vLookAt, fFOV, vColor));
      assert(m_spCorrespondingRenderingNode_Model);

      m_spCorrespondingRenderingNode_Model->SetGraphicCore(m_pTranslator->m_pCore);

      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode_Model);
      m_pTranslator->m_spDeferredNode->AddSpotLight(m_spCorrespondingRenderingNode);
    }
}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Light_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  Light_RuleObject *pNewObject = new Light_RuleObject();

  pNewObject->m_spSemNode = std::dynamic_pointer_cast<Light_SemSceneNode>(spSemNode);
  pNewObject->m_pTranslator = pTranslator;

  return pNewObject;
}
