// author: christian moellinger. comments htbd.

#ifndef __bambooengine_cube_header
#define __bambooengine_cube_header

#include "Graphics/Graphic.h"
#include "Graphics/SceneObjects/ISceneObject.h"

class Graphic::SO_Cube : public Graphic::ISceneObject
{
public:
    static std::shared_ptr<SO_Cube> Create();
    virtual std::shared_ptr<Graphic::IRenderNode> CreateRenderNode();
};

#endif
