#include "SemanticSceneNodes/Camera_SemSceneNode.h"

#include <cassert>

// register class at core system and get unique class id
BambooLib::t_classID Camera_SemSceneNode::s_ClassID = BambooLib::CoreSystem::GetInstance()->RegisterClass("SM_CAMER", NULL);

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
Camera_SemSceneNode::Camera_SemSceneNode() : ISemanticSceneNode(ClassID()), IIdentifyable(ClassID())
{
}

Camera_SemSceneNode::~Camera_SemSceneNode()
{
}
