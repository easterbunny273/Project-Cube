/*
 * header file for DeferredNodeTranslator::FractalTerrain_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 03/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_FRACTALTERRAINRULEOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_FRACTALTERRAINRULEOBJECT_DEFERREDTRANSLATOR_HEADER

#include "common_gl.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/FractalTerrain_SemSceneNode.h"
#include <memory>
#include "GeometryData.h"

class DeferredNodeTranslator::FractalTerrain_RuleObject : public DeferredNodeTranslator::IRuleObject
{
public:
  virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode,
                                  DeferredNodeTranslator *pTranslator);

  /// update the rendering scene graph pieces which correspond to the given semantic scene node
  virtual void Action();

  /// returns the list of accepted semantic scene nodes
  std::vector<ISemanticSceneNode::t_classID> GetAcceptedNodeIDs() const;

private:

  std::shared_ptr<GeometryData::GenericObject> ItlGenerateMeshFromFile(std::string sFilename);

  std::shared_ptr<Bamboo::RN_Generic>      m_spCorrespondingRenderingNode;
  std::shared_ptr<Bamboo::IRenderNode>      m_spCorrespondingRenderingNode_Model;

  std::shared_ptr<FractalTerrain_SemSceneNode> m_spSemNode;
};

#endif
