/*
 * header file for DeferredNodeTranslator::Cube_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_CUBERULEOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_CUBERULEOBJECT_DEFERREDTRANSLATOR_HEADER

#include "common_gl.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"
#include "BambooLib/include/GeneralDefinitions.h"
#include <memory>

class DeferredNodeTranslator::Cube_RuleObject : public DeferredNodeTranslator::IRuleObject
{
public:
  virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator);

  /// update the rendering scene graph pieces which correspond to the given semantic scene node
  virtual void Action();

  /// returns the list of accepted semantic scene nodes
  std::vector<BambooLib::t_classID> GetAcceptedNodeIDs() const;

private:
std::shared_ptr<Bamboo::RN_Generic>      m_spCorrespondingRenderingNode;
std::shared_ptr<Cube_SemSceneNode> m_spSemNode;
};

#endif
