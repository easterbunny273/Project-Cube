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
      std::shared_ptr<Bamboo::ICamera> spCamera = m_spSemNode->GetCamera();

      m_pTranslator->m_spRootNode = std::shared_ptr<Bamboo::IRenderNode>(new Bamboo::RN_Camera(spCamera.get()));

      m_pTranslator->m_spRootNode->SetInitialViewportInformation(1024,768);

      std::shared_ptr<Bamboo::RN_Deferred> spDeferredNode(new Bamboo::RN_Deferred(1024,768, false));

      spDeferredNode->SetGraphicCore(m_pTranslator->m_pCore);

      m_pTranslator->m_spDeferredNode = spDeferredNode;
      m_pTranslator->m_spRootNode->AddChild(spDeferredNode);
  }

  m_spSemNode->GetCamera()->Move(0.0001f);
}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Camera_RuleObject::CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator)
{
  Camera_RuleObject *pNewObject = new Camera_RuleObject();

  pNewObject->m_spSemNode = std::dynamic_pointer_cast<Camera_SemSceneNode>(spSemNode);;
  pNewObject->m_pTranslator = pTranslator;

  return pNewObject;
}
