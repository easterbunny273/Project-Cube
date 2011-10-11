// author: christian moellinger. comments htbd.

#ifndef __bambooengine_loadedmodel_header
#define __bambooengine_loadedmodel_header

#include "Graphics/Graphic.h"
#include "Graphics/SceneObjects/ISceneObject.h"

class Graphic::SO_LoadedModel : public Graphic::ISceneObject
{
private:
    std::string m_sFilename;

public:
    static std::shared_ptr<SO_LoadedModel> Create(std::string sFilename);

    virtual std::shared_ptr<Graphic::IRenderNode> CreateRenderNode();
};

#endif
