#ifndef __HEADER_ISCENEOBJECT_BAMBOO
#define __HEADER_ISCENEOBJECT_BAMBOO

#include "Graphics/Graphic.h"

class Graphic::ISceneObject
{
    friend class Scene;

public:
    void SetTransformMatrix(glm::mat4 mNewMatrix);
    glm::mat4 GetTransformMatrix() const;

    virtual ~ISceneObject() {}

    std::shared_ptr<Graphic::IRenderNode> GetRenderNode();
    virtual std::shared_ptr<Graphic::IRenderNode> CreateRenderNode() = 0;
protected:
    void SetScene(std::weak_ptr<Scene> wpScene);
    ISceneObject() {}

    std::shared_ptr<Graphic::IRenderNode>    m_spRenderNode;
    std::weak_ptr<Scene> m_wpScene;
};

#endif
