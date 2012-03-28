#include "RenderEngine-UE/Render.h"
#include "SemanticSceneNodes/GenericObject_SemSceneNode.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

#include <iostream>

extern std::shared_ptr<Light_SemSceneNode> g_spTestLight1;

void RenderEngineUE_Rendering::UpdateSemanticSceneGraph(std::set<GeometryData::GenericObject *> spObjects, std::shared_ptr<ISemanticSceneNode> spParentNode)
{
  static std::map<GeometryData::GenericObject *, std::shared_ptr<ISemanticSceneNode> > mCachedSemNodes;

  // remove all children from given node in scene graph
  spParentNode->ClearChildren();

  // add objects to semantic scene graph, create new semantic scene node if not cached yet, else only use the already cached node

 // std::cout << "draw " << spObjects.size() << " nodes" << std::endl;

  for (std::set<GeometryData::GenericObject *>::iterator iter=spObjects.begin();
       iter != spObjects.end();
       iter++)
    {
      std::map<GeometryData::GenericObject *, std::shared_ptr<ISemanticSceneNode> >::iterator cachedSemNode = mCachedSemNodes.find(*iter);

      if (cachedSemNode != mCachedSemNodes.end())
        {
          spParentNode->AddChild(cachedSemNode->second);
        }
      else
        {
          std::shared_ptr<ISemanticSceneNode> spNewSemNode(GenericObject_SemSceneNode::Create(*iter));

          mCachedSemNodes[*iter] = spNewSemNode;

          spParentNode->AddChild(spNewSemNode);
        }

    }
}
