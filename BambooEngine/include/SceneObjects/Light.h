// author: christian moellinger. comments htbd.

#ifndef __BAMBOOENGINE_SCENEOBJECT_LIGHT_HEADER
#define __BAMBOOENGINE_SCENEOBJECT_LIGHT_HEADER

#include "Graphic.h"
#include "SceneObjects/ISceneObject.h"

class Bamboo::SO_ILight : public Bamboo::ISceneObject
{
protected:
    /// method to return a render node
    virtual std::shared_ptr<Bamboo::IRenderNode> CreateRenderNode() = 0;
};

class Bamboo::SO_SpotLight : public Bamboo::SO_ILight
{
public:
    static std::shared_ptr<SO_SpotLight> Create(glm::vec3 vPosition,
                                                glm::vec3 vLookDirection,
                                                float fFOV,
                                                glm::vec3 vLightColor);

    virtual std::shared_ptr<Bamboo::IRenderNode> CreateRenderNode();
    virtual std::shared_ptr<Bamboo::IRenderNode> CreateRenderNodeModel();

private:
    /*! \name Construction */
    //@{
    SO_SpotLight();
    //@}

    /*! \name Private members */
    //@{
    glm::vec3   m_vPosition;
    glm::vec3   m_vLookDirection;
    float       m_fFieldOfView;
    glm::vec3   m_vColor;
    //@}
};

#endif
