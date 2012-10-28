#include "DeferredNodeTranslator/RuleObjects/Camera_RuleObject.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"

#include "DeferredNodeTranslator/RenderNodes/RenderNode_Camera.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"

namespace BambooGraphics
{
std::vector<BambooLib::t_classID> DeferredNodeTranslator::Camera_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<BambooLib::t_classID> vAcceptedIDs;

  // this rule object only accepts Camera_SemSceneNode
  vAcceptedIDs.push_back(Camera_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Camera_RuleObject::Action()
{
  bool bRootAlreadyCreated = (m_pTranslator->m_spRootNode.get() != NULL);

  if (!bRootAlreadyCreated)
  {
      std::shared_ptr<GraphicsCore::ICamera> spCamera = m_pSemNodeCamera->GetCamera();

      m_pTranslator->m_spRootNode = std::shared_ptr<GraphicsCore::IRenderNode>(new RN_Camera(spCamera.get()));

      m_pTranslator->m_spRootNode->SetInitialViewportInformation(1024,768);

      std::shared_ptr<RN_Deferred> spDeferredNode(new RN_Deferred(1024,768, false));

      spDeferredNode->SetGraphicCore(m_pTranslator->m_pCore);

      m_pTranslator->m_spDeferredNode = spDeferredNode;
      m_pTranslator->m_spRootNode->AddChild(spDeferredNode);
  }

  m_pSemNodeCamera->GetCamera()->Move(0.0001f);
}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Camera_RuleObject::CloneFor(ISemanticSceneNode *pSemNode, DeferredNodeTranslator *pTranslator)
{
  Camera_RuleObject *pNewObject = new Camera_RuleObject();

  pNewObject->m_pSemNodeCamera = Camera_SemSceneNode::Cast(pSemNode);
  pNewObject->m_pTranslator = pTranslator;

  return pNewObject;
}

}
