#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

#include "SemanticSceneNodes/Light_SemSceneNode.h"

std::shared_ptr<Light_SemSceneNode> Light_SemSceneNode::Create(float fFOV,
                                                               glm::vec3 vLightColor,
                                                               float fNearplane,
                                                               float fFarplane)
{
  // create new node
  Light_SemSceneNode * pNewNode = new Light_SemSceneNode();
  assert (pNewNode != NULL);

  // set initial parameters
  pNewNode->SetLightParameters(fFOV, vLightColor, fNearplane, fFarplane);

  // create shared_ptr
  std::shared_ptr<Light_SemSceneNode> spNewNode(pNewNode);

  // return shared ptr
  return spNewNode;
}

ISemanticSceneNode::t_classID Light_SemSceneNode::ClassID()
{
  return 2;
}

void Light_SemSceneNode::SetLightParameters(float fFOV,
                                            glm::vec3 vLightColor,
                                            float fNearplane,
                                            float fFarplane)
{
  m_fFOV        = fFOV;
  m_vLightColor = vLightColor;
  m_fNearplane  = fNearplane;
  m_fFarplane   = fFarplane;
}

void Light_SemSceneNode::SetTransformationMatrixByLookAtParameters(glm::vec3 vPosition,
                                                                   glm::vec3 vLookDirection,
                                                                   glm::vec3 vUp)
{
  // calc new transformation matrix
  glm::mat4 mNewTransformationMatrix = glm::lookAt(vPosition, vPosition + vLookDirection, vUp);

  // set
  SetTransformMatrix(mNewTransformationMatrix);
}

void Light_SemSceneNode::GetLightParameters(float &rfFOV,
                                           glm::vec3 &rvLightColor,
                                           float &rfNearplane,
                                           float &rfFarplane)
{
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
