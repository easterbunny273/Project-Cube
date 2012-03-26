#ifndef BAMBOO_RENDERENGINE_UE_RENDERING
#define BAMBOO_RENDERENGINE_UE_RENDERING

#include "GeometryData.h"
#include "Graphic.h"
#include "SemanticSceneNodes/ISemanticSceneNode.h"

#include <set>
#include <memory>

class RenderEngineUE_Rendering
{
public:
  void UpdateSemanticSceneGraph(std::set<GeometryData::GenericObject *> spObjects, std::shared_ptr<ISemanticSceneNode> spGraphRootNode);
};

#endif
