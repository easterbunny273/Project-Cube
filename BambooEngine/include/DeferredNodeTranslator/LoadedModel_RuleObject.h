/*
 * header file for DeferredNodeTranslator::LoadedModel_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_LOADEDMODELOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_LOADEDMODELOBJECT_DEFERREDTRANSLATOR_HEADER

#include "common_gl.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
#include <memory>

class DeferredNodeTranslator::LoadedModel_RuleObject : public DeferredNodeTranslator::IRuleObject
{
public:
  virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator);

  /// update the rendering scene graph pieces which correspond to the given semantic scene node
  virtual void Action();

  /// returns the list of accepted semantic scene nodes
  std::vector<ISemanticSceneNode::t_classID> GetAcceptedNodeIDs() const;

private:
  std::shared_ptr<Bamboo::RN_Generic>      m_spCorrespondingRenderingNode;
  std::shared_ptr<LoadedModel_SemSceneNode> m_spSemNode;

  std::shared_ptr<Bamboo::ICamera>      m_spCubemapCamera;
  std::shared_ptr<Bamboo::RN_Camera>    m_spCubemapCameraNode;
  std::shared_ptr<Bamboo::RN_Deferred>  m_spCubemapDeferredNode;

  bool  m_bEnvironmentalNodesCreated;
};

#endif
