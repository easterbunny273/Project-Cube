#include "Gamelogic/Objects/LightObject.h"

LightObject::LightObject(std::string sName)
{
	m_sName = sName;
	m_spLightSceneNode = Light_SemSceneNode::Create(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 50.0f, glm::vec3(1.0, 1.0, 1.0), 0.1, 50.0f);
}

LightObject::~LightObject()
{
}

std::string LightObject::GetObjectType()
{
	return "Light";
}

glm::mat4 LightObject::GetTransformation()
{
	return m_spLightSceneNode->GetTransformMatrix();
}

std::shared_ptr<ISemanticSceneNode> LightObject::GetSceneNode()
{
	return m_spLightSceneNode;
}

std::string LightObject::GetName()
{
	return m_sName;
}


void LightObject::SetPosition(const float fX, const float fY, const float fZ)
{
	m_spLightSceneNode->SetPosition(glm::vec3(fX, fY, fZ));
}

void LightObject::SetLookDirection(const float fX, const float fY, const float fZ)
{
	m_spLightSceneNode->SetLookDirection(glm::vec3(fX, fY, fZ));
}

void LightObject::SetFOV(const float fFOV)
{
	m_spLightSceneNode->SetFOV(fFOV);
}

void LightObject::SetColor(const float fR, const float fG, const float fB)
{
	m_spLightSceneNode->SetColor(glm::vec3(fR, fG, fB));
}

void LightObject::SetNearplane(const float fNearplane)
{
	m_spLightSceneNode->SetNearplane(fNearplane);
}

void LightObject::SetFarplane(const float fFarplane)
{
	m_spLightSceneNode->SetFarplane(fFarplane);
}
