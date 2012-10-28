#include "Gamelogic/Objects/Object.h"

Object::Object(std::string sName, std::string sFilename)
{
	m_sName = sName;
    m_pObjectSceneNode = LoadedModel_SemSceneNode::Create(sFilename);
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
    return m_pObjectSceneNode->GetTransformMatrix();
}

ISemanticSceneNode * Object::GetSceneNode()
{
    return m_pObjectSceneNode;
}

std::string Object::GetName()
{
	return m_sName;
}

void Object::Translate(float fX, float fY, float fZ)
{
    m_pObjectSceneNode->SetTransformMatrix(glm::translate(m_pObjectSceneNode->GetTransformMatrix(), glm::vec3(fX, fY, fZ)));
}

void Object::Scale(float fX, float fY, float fZ)
{
    m_pObjectSceneNode->SetTransformMatrix(glm::scale(m_pObjectSceneNode->GetTransformMatrix(), glm::vec3(fX, fY, fZ)));
}

void Object::ActivateEnvironmentMapping()
{
    m_pObjectSceneNode->ActivateEnvironmentMapping();
}

void Object::DeactivateEnvironmentMapping()
{
    m_pObjectSceneNode->DeactivateEnvironmentMapping();
}
