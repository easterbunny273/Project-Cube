#include "DeferredNodeTranslator/RuleObjects/LoadedModel_RuleObject.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Generic.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Camera.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Deferred.h"
#include "Camera.h"
#include "AssimpWrapper.h"

namespace BambooGraphics
{
std::vector<BambooLib::t_classID> DeferredNodeTranslator::LoadedModel_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<BambooLib::t_classID> vAcceptedIDs;

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
      m_spCorrespondingRenderingNode->SetTransformMatrix(m_pSemNode->GetTransformMatrix());

      // add rendering nodes, if environment mapping is activated but not handled yet
      if (!m_spCubemapDeferredNode && m_pSemNode->GetEnvironmentMapping())
      {
          m_spCubemapCamera = GraphicsCore::PerspectiveCamera::Create(90.0f, 1.0f, 0.001f, 100.0f, glm::vec3(), 0.0f, 0.0f);

          m_spCubemapCameraNode = std::shared_ptr<RN_Camera> (new RN_Camera(m_spCubemapCamera.get()));

          m_spCubemapDeferredNode = std::shared_ptr<RN_Deferred> (new RN_Deferred(128, 128, true));

          m_spCubemapCameraNode->AddChild(m_spCubemapDeferredNode);

          GLuint nDeferredAlbedoTexture = m_spCubemapDeferredNode->GetAlbedoTexture();
          m_spCorrespondingRenderingNode->SetEnvironmentMapping(true, nDeferredAlbedoTexture);

          m_pTranslator->m_spRootNode->AddChild(m_spCubemapCameraNode);
      }

      // remove rendering nodes, if environment mapping is deactivated but notes created
      if (m_spCubemapDeferredNode && !m_pSemNode->GetEnvironmentMapping())
      {
          m_pTranslator->m_spRootNode->RemoveChild(m_spCubemapCameraNode);

          m_spCubemapCamera.reset();
          m_spCubemapCameraNode.reset();
          m_spCubemapDeferredNode.reset();

          m_spCorrespondingRenderingNode->SetEnvironmentMapping(false, 0);
      }

      if (m_spCubemapDeferredNode)
      {
          glm::mat4 mTrans = m_pSemNode->GetTransformMatrix();

          m_spCubemapCamera->SetPosition(glm::vec3(mTrans[3][0], mTrans[3][1], mTrans[3][2]));
          //m_spCubemapCamera->SetPosition(glm::vec3(mTrans[0][3], mTrans[1][3], mTrans[2][3]));
         // m_spCubemapCamera->SetPosition(glm::vec3(0.0, 0.0, 0.0));

        m_spCubemapDeferredNode->ClearChilds();
        for (unsigned int i=0; i < m_pTranslator->m_vShadowCasterNodes.size(); i++)
        {
            if (m_pTranslator->m_vShadowCasterNodes[i].get() != m_spCorrespondingRenderingNode.get())
              m_spCubemapDeferredNode->AddChild(m_pTranslator->m_vShadowCasterNodes[i]);
        }
      }
    }
  else
    {
      // create new node
      m_spCorrespondingRenderingNode = std::shared_ptr<RN_Generic>(new RN_Generic(AssimpWrapper::LoadModel(m_pSemNode->GetFilename())));
      assert (m_spCorrespondingRenderingNode);

      m_pTranslator->m_vShadowCasterNodes.push_back(m_spCorrespondingRenderingNode);

      m_spCorrespondingRenderingNode->SetGraphicCore(m_pTranslator->m_pCore);

      // attach to deferred node
      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);

      m_bEnvironmentalNodesCreated = false;
    }

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::LoadedModel_RuleObject::CloneFor(ISemanticSceneNode *pSemNode, DeferredNodeTranslator *pTranslator)
{
  LoadedModel_RuleObject *pNewObject = new LoadedModel_RuleObject();

  pNewObject->m_pTranslator = pTranslator;

  pNewObject->m_pSemNode = LoadedModel_SemSceneNode::Cast(pSemNode);
  assert (pNewObject->m_pSemNode);


  return pNewObject;
}

}
