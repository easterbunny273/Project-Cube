#ifndef __BAMBOO_SCENE_HEADER
#define __BAMBOO_SCENE_HEADER

#include "Graphic.h"

class Bamboo::Scene
{
    friend class Bamboo;

public:
    static std::shared_ptr<Scene> Create();

    void AttachObject(std::shared_ptr<Bamboo::ISceneObject> spSceneObject);

private:
    std::list<std::shared_ptr<Bamboo::ISceneObject> >   m_lSceneObjects;
};

#endif
