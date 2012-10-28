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
#include <list>
#include <glm/glm.hpp>              // uses GLM
#include <glm/ext.hpp>

#include "BambooLib/include/IIdentifyable.h"

class INodeState;

class ISemanticSceneNode : virtual public BambooLib::IIdentifyable
{
public:
  /*! \name Public types */
  //@{
    /// list of child nodes
    typedef std::vector<ISemanticSceneNode* > t_children_vector;

    /// const vector of child nodes
    typedef const t_children_vector t_const_children_vec;

    class IListener
    {
    public:
        /// Triggered when the transformation matrix has changed
        virtual void OnTransformMatrixChanged(ISemanticSceneNode *pNode, glm::mat4 mNewMatrix);

        /// Triggered when a parameter (except the transformation matrix) has changed
        virtual void OnParametersHaveChanged(ISemanticSceneNode *pNode);

        /// Triggered when a child is added to the node
        virtual void OnChildAdded(ISemanticSceneNode *pNode, ISemanticSceneNode *pNewChild);

        /// Triggered when a child was removed from the node
        virtual void OnChildRemoved(ISemanticSceneNode *pNode, ISemanticSceneNode *pRemovedChild);
    };

  //@}


  /*! \name IIdentifyable interface */
  //@{
    static ISemanticSceneNode * Cast(BambooLib::IIdentifyable *pObject) { return dynamic_cast<ISemanticSceneNode *>(pObject); }
  //@}

  /*! \name Child nodes */
  //@{
    /// adds a child node
    void    AddChild(ISemanticSceneNode *pNode);

    /// Removes a child node
    void    RemoveChild(ISemanticSceneNode *pNode);

    /// returns a (const) vector of the children of this node
    t_const_children_vec * GetChildren();

    /// Returns the number of child nodes
    unsigned int GetNumChildNodes() { return m_vpChildren.size(); }

    /// Returns the given child node
    ISemanticSceneNode * GetChild(unsigned int nIndex);
  //@}

    /*! \name Listeners */
    //@{
        /// Registers a new listener which receives notifications
        void RegisterListener(IListener *pListener);

        /// Unregisters a listener
        void UnregisterListener(IListener *pListener);
    //@}

  /*! \name Transformation */
  //@{
    /// sets the transformation matrix (position + rotation)
    void  SetTransformMatrix(glm::mat4 mTransform);

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

    /*! \name Construction / Destruction */
    //@{
        void ItlTriggerOnTransformMatrixChanged();
        void ItlTriggerOnParametersHaveChanged();
        void ItlTriggerOnChildAdded(ISemanticSceneNode *pNewNode);
        void ItlTriggerOnChildRemoved(ISemanticSceneNode *pRemovedChild);
    //@}

    /*! \name Private members */
    //@{
      t_children_vector  m_vpChildren;
      glm::mat4       m_mTransformMatrix;
      std::list<IListener *> m_lpListener;
    //@}
};

#endif
