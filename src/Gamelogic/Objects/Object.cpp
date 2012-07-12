#include "Gamelogic/Objects/Object.h"

Object::Object(std::string sName, std::string sFilename)
{
	m_sName = sName;
	m_spObjectSceneNode = LoadedModel_SemSceneNode::Create(sFilename);
}

Object::~Object()
{
}

std::string Object::GetObjectType()
{
	return "LoadedModel";
}

glm::mat4 Object::GetTransformation()
{
	return m_spObjectSceneNode->GetTransformMatrix();
}

std::shared_ptr<ISemanticSceneNode> Object::GetSceneNode()
{
	return m_spObjectSceneNode;
}

std::string Object::GetName()
{
	return m_sName;
}

void Object::Translate(float fX, float fY, float fZ)
{
	m_spObjectSceneNode->SetTransformMatrix(glm::translate(m_spObjectSceneNode->GetTransformMatrix(), glm::vec3(fX, fY, fZ)));
}

void Object::Scale(float fX, float fY, float fZ)
{
	m_spObjectSceneNode->SetTransformMatrix(glm::scale(m_spObjectSceneNode->GetTransformMatrix(), glm::vec3(fX, fY, fZ)));
}

void Object::ActivateEnvironmentMapping()
{
	m_spObjectSceneNode->ActivateEnvironmentMapping();
}

void Object::DeactivateEnvironmentMapping()
{
	m_spObjectSceneNode->DeactivateEnvironmentMapping();
}
