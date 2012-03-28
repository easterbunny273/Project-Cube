#include "RenderEngine-UE/Render.h"
#include "SemanticSceneNodes/GenericObject_SemSceneNode.h"
#include "SemanticSceneNodes/Light_SemSceneNode.h"

#include <algorithm>

extern std::shared_ptr<Light_SemSceneNode> g_spTestLight1;

void RenderEngineUE_Rendering::UpdateSemanticSceneGraph(std::set<GeometryData::GenericObject *> spObjects, std::shared_ptr<ISemanticSceneNode> spParentNode)
{
  // cache for semantic scene nodes
  static std::map<GeometryData::GenericObject *, std::shared_ptr<ISemanticSceneNode> > mCachedSemNodes;

  // remove all children from given node in scene graph
  spParentNode->ClearChildren();

  // add objects to semantic scene graph, create new semantic scene node if not cached yet, else only use the already cached node
  for (std::set<GeometryData::GenericObject *>::iterator iter=spObjects.begin();
       iter != spObjects.end();
       iter++)
    {
      // search for cached semantic node
      std::map<GeometryData::GenericObject *, std::shared_ptr<ISemanticSceneNode> >::iterator cachedSemNode = mCachedSemNodes.find(*iter);

      if (cachedSemNode != mCachedSemNodes.end())
        {
          // add cached sem node to semantic scene graph
          spParentNode->AddChild(cachedSemNode->second);
        }
      else
        {
          // create new semantic scene node
          std::shared_ptr<ISemanticSceneNode> spNewSemNode(GenericObject_SemSceneNode::Create(*iter));

          // cache it
          mCachedSemNodes[*iter] = spNewSemNode;

          // add to semantic scene graph
          spParentNode->AddChild(spNewSemNode);
        }

    }
}
