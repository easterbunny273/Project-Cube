/*
 * header file for interface ISemanticSceneNode
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 02/2012 Project "BambooEngine"
 */

#pragma once

#ifndef __HEADER_ISEMANTICSCENENODE_BAMBOOENGINE
#define __HEADER_ISEMANTICSCENENODE_BAMBOOENGINE

#include <vector>
#include <memory>
#include <glm/glm.hpp>              // uses GLM
#include <glm/ext.hpp>

#include "BambooLib/include/IIdentifyable.h"

class INodeState;

class ISemanticSceneNode : virtual public BambooLib::IIdentifyable
{
public:
  /*! \name Public types */
  //@{
    /// vector of child nodes
    typedef std::vector<std::shared_ptr<ISemanticSceneNode> > t_children_vec;

    /// const vector of child nodes
    typedef const std::vector<std::shared_ptr<ISemanticSceneNode> > t_const_children_vec;
  //@}


  /*! \name IIdentifyable interface */
  //@{
    static ISemanticSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<ISemanticSceneNode *>(pObject); }
  //@}

  /*! \name Child nodes */
  //@{
    /// adds a child node
    void    AddChild(std::shared_ptr<ISemanticSceneNode> spNode) { m_vpChildren.push_back(spNode); }

    /// returns a (const) vector of the children of this node
    t_const_children_vec * GetChildren() { return &m_vpChildren; }
  //@}

  /*! \name Transformation */
  //@{
    /// sets the transformation matrix (position + rotation)
    void  SetTransformMatrix(glm::mat4 mTransform) { m_mTransformMatrix = mTransform; }

    /// returns the transformation matrix (position + rotation)
    glm::mat4 GetTransformMatrix() const { return m_mTransformMatrix; }
  //@}

protected:
    /*! \name Construction / Destruction */
    //@{
      /// constructor
      ISemanticSceneNode(BambooLib::t_classID nClassID) : IIdentifyable(nClassID) {}

      /// destructor
      virtual ~ISemanticSceneNode() {}
    //@}

    /*! \name Private members */
    //@{
      t_children_vec  m_vpChildren;
      glm::mat4       m_mTransformMatrix;
    //@}
};

#endif
