/*
 * header file for DeferredNodeTranslator::Cube_RuleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_CUBERULEOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_CUBERULEOBJECT_DEFERREDTRANSLATOR_HEADER

#include "GLUtils.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/Cube_SemSceneNode.h"
#include "BambooLib/include/GeneralDefinitions.h"
#include "DeferredNodeTranslator/RenderNodes/RenderNode_Generic.h"
#include <memory>

namespace BambooGraphics
{
    class DeferredNodeTranslator::Cube_RuleObject : public DeferredNodeTranslator::IRuleObject
    {
    public:
      virtual IRuleObject * CloneFor(ISemanticSceneNode *pSemNode, DeferredNodeTranslator *pTranslator);

      /// update the rendering scene graph pieces which correspond to the given semantic scene node
      virtual void Action();

      /// returns the list of accepted semantic scene nodes
      std::vector<BambooLib::t_classID> GetAcceptedNodeIDs() const;

    private:
    std::shared_ptr<RN_Generic>      m_spCorrespondingRenderingNode;
    Cube_SemSceneNode * m_pSemNode;
    };

}

#endif
