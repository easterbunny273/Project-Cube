#include "RenderNodes/RenderNode_SpotLight.h"
#include "RenderNodes/RenderNode_SpotLight_Model.h"
#include "SceneObjects/Light.h"

#include <memory>

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_SpotLight::CreateRenderNode()
{
    Bamboo::RN_SpotLight *pLightNode = new Bamboo::RN_SpotLight(m_vPosition, m_vLookDirection, m_fFieldOfView, m_vColor);

    return std::shared_ptr<Bamboo::IRenderNode>(pLightNode);
}

std::shared_ptr<Bamboo::IRenderNode> Bamboo::SO_SpotLight::CreateRenderNodeModel()
{
    Bamboo::RN_SpotLight_Model *pLightNode = new Bamboo::RN_SpotLight_Model(m_vPosition, m_vLookDirection, m_fFieldOfView, m_vColor);

    return std::shared_ptr<Bamboo::IRenderNode>(pLightNode);
}

Bamboo::SO_SpotLight::SO_SpotLight()
{
    // nothing to do so far
}

std::shared_ptr<Bamboo::SO_SpotLight> Bamboo::SO_SpotLight::Create(glm::vec3 vPosition,
                                                                   glm::vec3 vLookDirection,
                                                                   float fFOV,
                                                                   glm::vec3 vLightColor)
{
    SO_SpotLight *pNewObject = new SO_SpotLight();

    pNewObject->m_vPosition     = vPosition;
    pNewObject->m_fFieldOfView  = fFOV;
    pNewObject->m_vLookDirection = vLookDirection;
    pNewObject->m_vColor        = vLightColor;

    return std::shared_ptr<Bamboo::SO_SpotLight>(pNewObject);
}
