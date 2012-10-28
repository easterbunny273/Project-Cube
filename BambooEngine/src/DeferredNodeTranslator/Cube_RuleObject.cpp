#include "DeferredNodeTranslator/RuleObjects/Cube_RuleObject.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Generic.h"

namespace BambooGraphics
{
std::vector<BambooLib::t_classID> DeferredNodeTranslator::Cube_RuleObject::GetAcceptedNodeIDs() const
{
  std::vector<BambooLib::t_classID> vAcceptedIDs;

  // this rule object only accepts Cube_SemSceneNode
  vAcceptedIDs.push_back(Cube_SemSceneNode::ClassID());

  return vAcceptedIDs;
}

void DeferredNodeTranslator::Cube_RuleObject::Action()
{
  assert (m_pTranslator->m_spRootNode);
  assert (m_pTranslator->m_spDeferredNode);

  if (m_spCorrespondingRenderingNode)
    {
      // only update values
      m_spCorrespondingRenderingNode->SetTransformMatrix(m_pSemNode->GetTransformMatrix());
    }
  else
    {
      Cube *pCube = m_pSemNode->GetCube();
      std::shared_ptr<GeometryData::GenericObject> spGenericObject = pCube->GenerateGenericObject();

      // create new node
      m_spCorrespondingRenderingNode = std::shared_ptr<RN_Generic>(new RN_Generic(spGenericObject));
      assert (m_spCorrespondingRenderingNode);

      m_pTranslator->m_vShadowCasterNodes.push_back(m_spCorrespondingRenderingNode);

      // set graphic core
      m_spCorrespondingRenderingNode->SetGraphicCore(m_pTranslator->m_pCore);

      // attach to deferred node
      m_pTranslator->m_spDeferredNode->AddChild(m_spCorrespondingRenderingNode);
    }

}

DeferredNodeTranslator::IRuleObject *DeferredNodeTranslator::Cube_RuleObject::CloneFor(ISemanticSceneNode *pSemNode, DeferredNodeTranslator *pTranslator)
{
  Cube_RuleObject *pNewObject = new Cube_RuleObject();

  pNewObject->m_pTranslator   = pTranslator;
  pNewObject->m_pSemNode     = Cube_SemSceneNode::Cast(pSemNode);

  return pNewObject;
}

}
