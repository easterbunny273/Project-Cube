#include "DeferredNodeTranslator/Camera_RuleObject.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"

#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"

std::vector<ISemanticSceneNode::t_classID> DeferredNodeTranslator::Camera_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs;

  // this rule object only accepts Camera_SemSceneNode
  vAcceptedIDs.push_back(Camera_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Camera_RuleObject::Action()
{
  bool bRootAlreadyCreated = (m_pTranslator->m_spRootNode.get() != NULL);

  if (!bRootAlreadyCreated)
  {
      static std::shared_ptr<Bamboo::ICamera> spTestCamera = Bamboo::PerspectiveCamera::Create(45.0f, 1.33, 0.1, 100, glm::vec3(), 90, 0);

      m_pTranslator->m_spRootNode = std::shared_ptr<Bamboo::IRenderNode>(new Bamboo::RN_Camera(spTestCamera.get()));

      m_pTranslator->m_spRootNode->SetInitialViewportInformation(1024,768);

      std::shared_ptr<Bamboo::RN_Deferred> spDeferredNode(new Bamboo::RN_Deferred(1024,768, false));

      m_pTranslator->m_spDeferredNode = spDeferredNode;
      m_pTranslator->m_spRootNode->AddChild(spDeferredNode);
  }
}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Camera_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  Camera_RuleObject *pNewObject = new Camera_RuleObject();

  pNewObject->m_spSemNode = spSemNode;
  pNewObject->m_pTranslator = pTranslator;

  return pNewObject;
}
