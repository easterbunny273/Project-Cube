#include <glm/gtc/matrix_transform.hpp>
#include <cassert>
#include "SemanticSceneNodes/Light_SemSceneNode.h"

#include "ConstrainedKeyValueMap.h"

// register class at core system and get unique class id
BambooLib::t_classID Light_SemSceneNode::s_ClassID = BambooLib::CoreSystem::GetInstance()->RegisterClass("SM_LIGHT", NULL);

Light_SemSceneNode * Light_SemSceneNode::Create(glm::vec3 vPosition,
                                               glm::vec3 vLookDirection,
                                               float fFOV,
                                               glm::vec3 vLightColor,
                                               float fNearplane,
                                               float fFarplane)
{
  // create new node
  Light_SemSceneNode * pNewNode = new Light_SemSceneNode();
  assert (pNewNode != NULL);

  // set initial parameters
  pNewNode->SetLightParameters(vPosition, vLookDirection, fFOV, vLightColor, fNearplane, fFarplane);

  // return new object
  return pNewNode;
}

void Light_SemSceneNode::SetLightParameters(glm::vec3 vPosition,
                                            glm::vec3 vLookDirection,
                                            float fFOV,
                                            glm::vec3 vLightColor,
                                            float fNearplane,
                                            float fFarplane)
{
  m_vLightPosition = vPosition;
  m_vLightLookDirection = vLookDirection;
  m_fFOV        = fFOV;
  m_vLightColor = vLightColor;
  m_fNearplane  = fNearplane;
  m_fFarplane   = fFarplane;
}


void Light_SemSceneNode::GetLightParameters(glm::vec3 &rvPosition,
                                           glm::vec3 &rvLookDirection,
                                           float &rfFOV,
                                           glm::vec3 &rvLightColor, float &rfNearplane, float &rfFarplane)
{
  rvPosition  = m_vLightPosition;
  rvLookDirection = m_vLightLookDirection;
  rfFOV       = m_fFOV;
  rvLightColor= m_vLightColor;
  rfNearplane = m_fNearplane;
  rfFarplane  = m_fFarplane;
}

void Light_SemSceneNode::SetPosition(glm::vec3 vPosition)
{
	m_vLightPosition = vPosition;
}

void Light_SemSceneNode::SetLookDirection(glm::vec3 vLookDirection)
{
	m_vLightLookDirection = vLookDirection;
}

void Light_SemSceneNode::SetFOV(float fFOV)
{
	m_fFOV = fFOV;
}

void Light_SemSceneNode::SetColor(glm::vec3 vColor)
{
	m_vLightColor = vColor;
}

void Light_SemSceneNode::SetNearplane(float fNearplane)
{
	m_fNearplane = fNearplane;
}

void Light_SemSceneNode::SetFarplane(float fFarplane)
{
	m_fFarplane = fFarplane;
}

Light_SemSceneNode::Light_SemSceneNode() : ISemanticSceneNode(ClassID()), IIdentifyable(ClassID())
{
  // create parameter keyvaluemap with constraints

  // create constraints
  BambooLib::ConstrainedKeyValueMap::TConstrainMap mConstrains;
  mConstrains["field-of-view"] = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.1, 179.0, 0.5);
  mConstrains["nearplane"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.0001, 100000.0, 0.0001);
  mConstrains["farplane"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.0001, 100000.0, 0.0001);
  mConstrains["pos.x"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["pos.y"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["pos.z"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["look-direction.x"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["look-direction.y"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["look-direction.z"]     = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT);
  mConstrains["color.r"]    = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.0, 1.0, 0.001);
  mConstrains["color.g"]    = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.0, 1.0, 0.001);
  mConstrains["color.b"]    = BambooLib::ConstrainedKeyValueMap::CreateConstraint(BambooLib::KeyValueMap::VALUE_FLOAT, 0.0, 1.0, 0.001);

  // create parameters map
  m_pParameters = new BambooLib::ConstrainedKeyValueMap(mConstrains, false);
  assert (m_pParameters != NULL);

  // set default values
  bool bOk = true;
  bOk &= m_pParameters->SetValue("field-of-view", 45.0f);
  bOk &= m_pParameters->SetValue("nearplane", 0.1f);
  bOk &= m_pParameters->SetValue("farplane", 0.1f);
  bOk &= m_pParameters->SetValue("pos.x", 0.1f);
  bOk &= m_pParameters->SetValue("pos.y", 0.1f);
  bOk &= m_pParameters->SetValue("pos.z", 0.1f);
  bOk &= m_pParameters->SetValue("look-direction.x", 0.1f);
  bOk &= m_pParameters->SetValue("look-direction.y", 0.1f);
  bOk &= m_pParameters->SetValue("look-direction.z", 0.1f);
  bOk &= m_pParameters->SetValue("color.r", 1.0f);
  bOk &= m_pParameters->SetValue("color.g", 1.0f);
  bOk &= m_pParameters->SetValue("color.b", 1.0f);
  assert (bOk);     // if this assertion fails, some default variable does not fulfill the constraints
}

Light_SemSceneNode::~Light_SemSceneNode()
{
  // nothing to do
}

