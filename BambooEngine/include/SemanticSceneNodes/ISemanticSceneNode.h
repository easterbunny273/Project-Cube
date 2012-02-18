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

class INodeState;

class ISemanticSceneNode
{
public:
  /*! \name Public types */
  //@{
    /// class id
    typedef unsigned int t_classID;

    /// vector of child nodes
    typedef std::vector<std::shared_ptr<ISemanticSceneNode> > t_children_vec;

    /// const vector of child nodes
    typedef const std::vector<std::shared_ptr<ISemanticSceneNode> > t_const_children_vec;
  //@}


  /*! \name Class identification */
  //@{
    /// returns the class id of this instance
    t_classID GetClassID();

    /// returns the static class id, must be defined in each inherit class
    // static t_classID StaticClassID();

    /// returns if the class has the same type as the given one
    bool      IsClass(t_classID nOther) { return (nOther == m_nClassID); }
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
    glm::mat4 GetTransformationMatrix() const { return m_mTransformMatrix; }
  //@}

protected:
    /*! \name Construction / Destruction */
    //@{
      /// constructor
      ISemanticSceneNode(t_classID nClassID) : m_nClassID(nClassID) {}

      /// destructor
      virtual ~ISemanticSceneNode() {}
    //@}

    /*! \name Private members */
    //@{
      t_children_vec  m_vpChildren;
      t_classID       m_nClassID;
      glm::mat4       m_mTransformMatrix;
    //@}
};

#endif
