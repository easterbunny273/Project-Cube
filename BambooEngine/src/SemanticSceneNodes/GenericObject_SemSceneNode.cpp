#include "SemanticSceneNodes/GenericObject_SemSceneNode.h"
#include <iostream>

std::shared_ptr<GenericObject_SemSceneNode> GenericObject_SemSceneNode::Create(GeometryData::GenericObject *pObject)
{
  // create node
  GenericObject_SemSceneNode *pNode = new GenericObject_SemSceneNode();

  // set parameters
  pNode->m_spObject = std::shared_ptr<GeometryData::GenericObject>(pObject);

  // create shared_ptr
  std::shared_ptr<GenericObject_SemSceneNode> spNode(pNode);

  return spNode;
}

GenericObject_SemSceneNode::~GenericObject_SemSceneNode()
{
  // nothing to do
}

ISemanticSceneNode::t_classID GenericObject_SemSceneNode::ClassID()
{
  return 4;
}

GenericObject_SemSceneNode::GenericObject_SemSceneNode() : ISemanticSceneNode(ClassID())
{
  // initialise variables
  //m_spObject = NULL;
}

GeometryData::GenericObject * GenericObject_SemSceneNode::GetObject()
{
  return m_spObject.get();
}

std::shared_ptr<GeometryData::GenericObject> GenericObject_SemSceneNode::GetObjectSP()
{
  return m_spObject;
}

