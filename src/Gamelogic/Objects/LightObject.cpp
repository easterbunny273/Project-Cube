#include "Gamelogic\Objects\LightObject.h"

LightObject::LightObject()
{
	m_spLightSceneNode = Light_SemSceneNode::Create(glm::vec3(-0.2f, 0.10f, 0.14f), glm::vec3(1.0f, -0.4f, -1.0f), 50.0f, glm::vec3(1.0, 1.0, 1.0), 0.1, 50.0f);
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