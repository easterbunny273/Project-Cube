#include "Scene.h"
#include "RenderNodes/IRenderNode.h"
#include "RenderNodes/RenderNode_Camera.h"
#include "RenderNodes/RenderNode_PostEffect.h"
#include "RenderNodes/RenderNode_FBO.h"
#include "RenderNodes/RenderNode_AssimpImport.h"
#include "SceneObjects/ISceneObject.h"
#include "SceneObjects/Cube.h"
#include "SceneObjects/LoadedModel.h"
#include "ShaderManager.h"
#include "TextureManager.h"

std::shared_ptr<Bamboo::Scene> Bamboo::Scene::Create()
{
    std::shared_ptr<Bamboo::Scene> spNewScene(new Bamboo::Scene());

    return spNewScene;
}

void Bamboo::Scene::AttachObject(std::shared_ptr<Bamboo::ISceneObject> spSceneObject)
{
    m_lSceneObjects.push_back(spSceneObject);
}
