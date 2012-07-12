/*
 * header file for SampleObject class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 07/2011, Projekt CUBE
 */

#ifndef __HEADER_SAMPLEOBJECT
#define __HEADER_SAMPLEOBJECT

#include "Gamelogic/IGameState.h"
#include "Gamelogic/IObject.h"

#include <list>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
  *  This class is just a dummy class to provide some objects
  *  to the graphics engine.
  *
  *
  */

class SampleObject : public IObject
{
public:
    /*! \name IObject */
    //@{
    /// returns the object type as a string. used to define the class of
    /// objects and to determinate which model should be rendered
    virtual std::string GetObjectType();

    /// returns the current transformation matrix of the object
    virtual glm::mat4 GetTransformation();

	/// returns the semantic scene node of this object
	virtual std::shared_ptr<ISemanticSceneNode> GetSceneNode();

	/// returns the user defined name of the object
	virtual std::string GetName();

    //@}


};

#endif
