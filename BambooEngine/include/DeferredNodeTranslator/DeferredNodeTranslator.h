/*
 * header file for Bamboo::DeferredNodeTranslator class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_DEFERREDNODETRANSLATOR_HEADER
#define __BAMBOOENGINE_DEFERREDNODETRANSLATOR_HEADER

#include "INodeTranslator.h"
#include "RenderNodes/RenderNode_Deferred.h"

#include <memory>

/* A node translator generates pieces of render scene graph nodes or rather a
  rendering scene graph from a semantic scene graph, caches the results, and updates
  the necessary parts if some part in the application state has changed (due user input, for example).
  There may be different classes which inherit from the interface INodeTranslator, for example,
  one translator may generate a rendering graph using deferred shading.
*/

class DeferredNodeTranslator : public INodeTranslator
{
public:
  /*! \name Public types */
  //@{
    class IRuleObject
    {
    public:
      virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator) = 0;

      /// update the rendering scene graph pieces which correspond to the given semantic scene node
      virtual void Action() = 0;

      /// return a vector with the class ids of the semantic scene nodes, which were accepted (and handled) by this rule object
      virtual std::vector<ISemanticSceneNode::t_classID> GetAcceptedNodeIDs() const = 0;

      /// whether a given scene is accepted by this rule object
      bool IsAccepted(ISemanticSceneNode::t_classID nClassID) const
      {
        std::vector<ISemanticSceneNode::t_classID> vAcceptedIDs = GetAcceptedNodeIDs();
        for (unsigned int i=0; i < vAcceptedIDs.size(); i++)
          if (vAcceptedIDs[i] == nClassID)
            return true;

        return false;
      }

    protected:
      DeferredNodeTranslator * m_pTranslator;
      std::shared_ptr<ISemanticSceneNode> m_spSemNode;
    };
  //@}

  /*! \name Construction / Destruction */
  //@{
      DeferredNodeTranslator(Bamboo *pCore);
      ~DeferredNodeTranslator();
  //@}

  /*! \name Forward declarations */
  //@{
      class Camera_RuleObject;
      class Cube_RuleObject;
      class LoadedModel_RuleObject;
      class Light_RuleObject;

  //@}

  /*! \name INodeTranslator interface */
  //@{
    virtual void Translate(std::shared_ptr<ISemanticSceneNode> spSemRoot);
  //@}

private:
  /*! \name Internal methods */
  //@{
      void ItlRegisterRuleObjectPrototype(std::shared_ptr<IRuleObject> pObject);

      void ItlTranslateSemNode(std::shared_ptr<ISemanticSceneNode> spSemNode);
  //@}

  std::map<ISemanticSceneNode::t_objectID, std::shared_ptr<IRuleObject> > m_mCachedRuleObjects;

  std::map<ISemanticSceneNode::t_classID, std::shared_ptr<IRuleObject> > m_mRegisteredRuleObjects;

  std::vector<std::shared_ptr<Bamboo::IRenderNode> > m_vLightNodes;
  std::vector<std::shared_ptr<Bamboo::IRenderNode> > m_vShadowCasterNodes;

  std::shared_ptr<Bamboo::RN_Deferred>    m_spDeferredNode;
};

#endif
