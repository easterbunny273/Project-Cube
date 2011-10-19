// author: christian moellinger. comments htbd.

#ifndef __bambooengine_cube_header
#define __bambooengine_cube_header

#include "Graphics/Graphic.h"
#include "Graphics/SceneObjects/ISceneObject.h"

class Bamboo::SO_Cube : public Bamboo::ISceneObject
{
public:
    static std::shared_ptr<SO_Cube> Create();
    virtual std::shared_ptr<Bamboo::IRenderNode> CreateRenderNode();
};

#endif
