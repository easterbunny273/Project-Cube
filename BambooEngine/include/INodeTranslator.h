/*
 * header file for Bamboo::INodeTranslator class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012, Project "BambooEngine"
 */

#pragma once

#ifndef __BAMBOOENGINE_INODETRANSLATOR_HEADER
#define __BAMBOOENGINE_INODETRANSLATOR_HEADER

//#include "common_gl.h"
//#include "Graphic.h"
#include "RenderNodes/IRenderNode.h"

#include <memory>

/* A node translator generates pieces of render scene graph nodes or rather a
  rendering scene graph from a semantic scene graph, caches the results, and updates
  the necessary parts if some part in the application state has changed (due user input, for example).
  There may be different classes which inherit from the interface INodeTranslator, for example,
  one translator may generate a rendering graph using deferred shading.
*/

class INodeTranslator
{
public:
  virtual void Translate(std::shared_ptr<ISemanticSceneNode> spSemRoot) = 0;

  std::shared_ptr<Bamboo::IRenderNode> GetRenderGraph() { return m_spRootNode; }

  virtual ~INodeTranslator() {};

protected:
  INodeTranslator(Bamboo *pCore) { m_pCore = pCore; }

  std::shared_ptr<Bamboo::IRenderNode> m_spRootNode;
  Bamboo *m_pCore;
};

#endif
