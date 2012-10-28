/*
 * header file for DeferredNodeTranslator::LoadedModel_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_LOADEDMODELOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_LOADEDMODELOBJECT_DEFERREDTRANSLATOR_HEADER

#include "GLUtils.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/LoadedModel_SemSceneNode.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Camera.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Generic.h"
#include <memory>
#include "BambooLib/include/IIdentifyable.h"

namespace BambooGraphics
{
class DeferredNodeTranslator::LoadedModel_RuleObject : public DeferredNodeTranslator::IRuleObject
{
public:
  virtual IRuleObject * CloneFor(ISemanticSceneNode *pSemNode, DeferredNodeTranslator *pTranslator);

  /// update the rendering scene graph pieces which correspond to the given semantic scene node
  virtual void Action();

  /// returns the list of accepted semantic scene nodes
  std::vector<BambooLib::t_classID> GetAcceptedNodeIDs() const;

private:
  std::shared_ptr<RN_Generic>      m_spCorrespondingRenderingNode;
  LoadedModel_SemSceneNode  * m_pSemNode;

  std::shared_ptr<GraphicsCore::ICamera>      m_spCubemapCamera;
  std::shared_ptr<RN_Camera>    m_spCubemapCameraNode;
  std::shared_ptr<RN_Deferred>  m_spCubemapDeferredNode;

  bool  m_bEnvironmentalNodesCreated;
};

}

#endif
