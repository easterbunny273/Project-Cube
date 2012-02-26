#include "SemanticSceneNodes/Camera_SemSceneNode.h"

#include <cassert>

std::shared_ptr<Camera_SemSceneNode> Camera_SemSceneNode::Create(std::shared_ptr<Bamboo::ICamera> spCamera)
{
  // create new node
  Camera_SemSceneNode * pNewNode = new Camera_SemSceneNode();
  assert (pNewNode != NULL);

  // set initial parameters
  pNewNode->m_spCamera    = spCamera;

  // create shared_ptr
  std::shared_ptr<Camera_SemSceneNode> spNewNode(pNewNode);

  // return shared ptr
  return spNewNode;
}

ISemanticSceneNode::t_classID Camera_SemSceneNode::ClassID()
{
  return 0;
}
/*
void Camera_SemSceneNode::SetPerspectiveProjection(float fFOV,
                                                   float fRatio,
                                                   float fNearplane,
                                                   float fFarplane)
{
  m_fFOV        = fFOV;
  m_fRatio      = fRatio;
  m_fNearplane  = fNearplane;
  m_fFarplane   = fFarplane;
}

void Camera_SemSceneNode::GetPerspectiveProjection(float &rfFOV,
                                                   float &rfRatio,
                                                   float &rfNearplane,
                                                   float &rfFarplane)
{
  rfFOV       = m_fFOV;
  rfRatio     = m_fRatio;
  rfNearplane = m_fNearplane;
  rfFarplane  = m_fFarplane;
}
*/
Camera_SemSceneNode::Camera_SemSceneNode() : ISemanticSceneNode(ClassID())
{
}

Camera_SemSceneNode::~Camera_SemSceneNode()
{
}
