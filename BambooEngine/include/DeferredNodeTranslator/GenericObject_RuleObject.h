/*
 * header file for DeferredNodeTranslator::GenericObject_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 03/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_GENERICOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_GENERICOBJECT_DEFERREDTRANSLATOR_HEADER

#include "common_gl.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/GenericObject_SemSceneNode.h"

#include <memory>

class DeferredNodeTranslator::GenericObject_RuleObject : public DeferredNodeTranslator::IRuleObject
{
public:
  virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator);

  // IDeleteListener interface
  virtual void OnDelete(ISemanticSceneNode *pNode);
  virtual void OnRemovedFromParent(ISemanticSceneNode *pParent, ISemanticSceneNode *pNode);

  /// update the rendering scene graph pieces which correspond to the given semantic scene node
  virtual void Action();

  /// returns the list of accepted semantic scene nodes
  std::vector<ISemanticSceneNode::t_classID> GetAcceptedNodeIDs() const;

  GenericObject_RuleObject() {};
  virtual ~GenericObject_RuleObject();
protected:

        bool    m_bActive;

  std::shared_ptr<Bamboo::RN_Generic>      m_spCorrespondingRenderingNode;
  std::shared_ptr<GenericObject_SemSceneNode> m_spSemNode;
};

#endif
