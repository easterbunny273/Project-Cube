/*
 * header file for Bamboo::DeferredNodeTranslator class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_CAMERARULEOBJECT_DEFERREDTRANSLATOR_HEADER
#define __BAMBOOENGINE_CAMERARULEOBJECT_DEFERREDTRANSLATOR_HEADER

#include "common_gl.h"
#include "Graphic.h"
#include "DeferredNodeTranslator/DeferredNodeTranslator.h"
#include "SemanticSceneNodes/Camera_SemSceneNode.h"
#include "BambooLib/include/IIdentifyable.h"

#include <memory>

namespace BambooGraphics
{
    class DeferredNodeTranslator::Camera_RuleObject : public DeferredNodeTranslator::IRuleObject
    {
    public:
      virtual IRuleObject * CloneFor(std::shared_ptr<ISemanticSceneNode> spSemNode, DeferredNodeTranslator *pTranslator);

      /// update the rendering scene graph pieces which correspond to the given semantic scene node
      virtual void Action();

      /// returns the list of accepted semantic scene nodes
      std::vector<BambooLib::t_classID> GetAcceptedNodeIDs() const;

    private:
    std::shared_ptr<Camera_SemSceneNode> m_spSemNode;
    };

}

#endif
