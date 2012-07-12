/*
 * header file for IObject interface
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#pragma once
#ifndef __IOBJECT_PROJECTCUBE_HEADER
#define __IOBJECT_PROJECTCUBE_HEADER

#include <memory>
#include <list>
#include <string>

#include "SemanticSceneNodes/ISemanticSceneNode.h"

class IObject
{
public:
    /*! \name Public methods */
    //@{
    
	/// returns the object type as a string. used to define the class of
    /// objects and to determinate which model should be rendered
    virtual std::string GetObjectType() = 0;

    /// returns the current transformation matrix of the object
    virtual glm::mat4 GetTransformation() = 0;

	/// returns the semantic scene node of this object
	virtual std::shared_ptr<ISemanticSceneNode> GetSceneNode() = 0;

	/// returns the user defined name of the object
	virtual std::string GetName() = 0;

	/*
    /// returns the current animation state
    /// todo: think about other ideas? this way, only vertex
    /// shader animations are possible.
    /// but animations are something for later milestones, therefore commented out
    virtual float GetAnimationState() = 0;
    */

    //@}

};

#endif // __IOBJECT_PROJECTCUBE_HEADER
