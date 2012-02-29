#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

#include "SemanticSceneNodes/Light_SemSceneNode.h"

std::shared_ptr<Light_SemSceneNode> Light_SemSceneNode::Create(glm::vec3 vPosition,
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

  // create shared_ptr
  std::shared_ptr<Light_SemSceneNode> spNewNode(pNewNode);

  // return shared ptr
  return spNewNode;
}

ISemanticSceneNode::t_classID Light_SemSceneNode::ClassID()
{
  return 2;
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

Light_SemSceneNode::Light_SemSceneNode() : ISemanticSceneNode(ClassID())
{
  // nothing to do, only initializer list
}

Light_SemSceneNode::~Light_SemSceneNode()
{
  // nothing to do
}
