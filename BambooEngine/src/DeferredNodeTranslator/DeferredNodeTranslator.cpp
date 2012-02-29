#include "DeferredNodeTranslator/DeferredNodeTranslator.h"

#include "DeferredNodeTranslator/Camera_RuleObject.h"
#include "DeferredNodeTranslator/Cube_RuleObject.h"
#include "DeferredNodeTranslator/Light_RuleObject.h"
#include "DeferredNodeTranslator/LoadedModel_RuleObject.h"

#include "PC_Logger.h"

DeferredNodeTranslator::DeferredNodeTranslator(Bamboo *pCore) : INodeTranslator(pCore)
{
  std::shared_ptr<IRuleObject> spCameraRule(new Camera_RuleObject());
  std::shared_ptr<IRuleObject> spCubeRule(new Cube_RuleObject());
  std::shared_ptr<IRuleObject> spLightRule(new Light_RuleObject());
  std::shared_ptr<IRuleObject> spLoadedModelRule(new LoadedModel_RuleObject());

  ItlRegisterRuleObjectPrototype(spCameraRule);
  ItlRegisterRuleObjectPrototype(spCubeRule);
  ItlRegisterRuleObjectPrototype(spLightRule);
  ItlRegisterRuleObjectPrototype(spLoadedModelRule);
}

DeferredNodeTranslator::~DeferredNodeTranslator()
{

}

void DeferredNodeTranslator::Translate(std::shared_ptr<ISemanticSceneNode> spSemRoot)
{
  ItlTranslateSemNode(spSemRoot);

  ISemanticSceneNode::t_const_children_vec  * pvChildren = spSemRoot->GetChildren();

  for (ISemanticSceneNode::t_const_children_vec::const_iterator it = pvChildren->begin(); it != pvChildren->end(); it++)
    {
      Translate(*it);
    }

}

void DeferredNodeTranslator::ItlRegisterRuleObjectPrototype(std::shared_ptr<IRuleObject> pObject)
{
  std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs = pObject->GetAcceptedNodeIDs();

  for (unsigned int i=0; i < vAcceptedIDs.size(); i++)
  {
      bool bAlreadyRegistered = (m_mRegisteredRuleObjects.find(vAcceptedIDs[i]) != m_mRegisteredRuleObjects.end());

      assert (bAlreadyRegistered == false);

      if (bAlreadyRegistered)
        Logger::error() << "A rule object prototype for handling semantic scene graph nodes with class id " << vAcceptedIDs[i] << " is already registered!" << Logger::endl;
      else
        m_mRegisteredRuleObjects[vAcceptedIDs[i]] = pObject;
  }
}

void DeferredNodeTranslator::ItlTranslateSemNode(std::shared_ptr<ISemanticSceneNode> spSemNode)
{
  bool bRuleObjectExists = (m_mCachedRuleObjects.find(spSemNode->GetObjectID()) != m_mCachedRuleObjects.end());

  // if the rule object for this object id does not exist, create and cache it
  if (!bRuleObjectExists)
    {
      bool bFittingRuleObjectPrototypeExists = (m_mRegisteredRuleObjects.find(spSemNode->GetClassID()) != m_mRegisteredRuleObjects.end());

      if (!bFittingRuleObjectPrototypeExists)
        Logger::fatal() << "No rule object prototype fits on a given semantic scene node. Semantic scene node class id: " << spSemNode->GetClassID() << Logger::endl;
      else
        {
          std::shared_ptr<IRuleObject> spRulePrototype = m_mRegisteredRuleObjects[spSemNode->GetClassID()];

          std::shared_ptr<IRuleObject> spConcreteRuleObject(spRulePrototype->CloneFor(spSemNode, this));

          unsigned int nOldSize = m_mCachedRuleObjects.size();

          m_mCachedRuleObjects[spSemNode->GetObjectID()] = spConcreteRuleObject;

          // assert that size has changed
          assert (nOldSize != m_mCachedRuleObjects.size());
        }
  }

  // rule should now exist
  bool bRuleObjectExists2 = (m_mCachedRuleObjects.find(spSemNode->GetObjectID()) != m_mCachedRuleObjects.end());
  assert (bRuleObjectExists2);

  m_mCachedRuleObjects[spSemNode->GetObjectID()]->Action();
}
