/*
 * header file for IGameState interface
 * written by: christian moellinger <ch.moellinger@gmail.com>
 *             florian spechtenhauser <florian.spechtenhauser@gmail.com>
 *
 * 07/2011, Project "Project Cube"
 */

#pragma once
#ifndef __IGAMESTATE_PROJECTCUBE_HEADER
#define __IGAMESTATE_PROJECTCUBE_HEADER

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <list>

#include "Gamelogic/IObject.h"
#include "Gamelogic/ICube.h"

class IGameState
{
public:
    /*! \name Public methods */
    //@{
        /// returns a list of the objects in the current cube
	virtual std::list<IObject *> GetObjectsInCube() = 0;

        /// returns a shared_ptr to the currently active cube
        /// todo: is a shared_ptr necessary?
	virtual std::shared_ptr<ICube *> GetCurrentCube() = 0;

        /// returns the position of the player in the current cube
        /// todo: what's the reference point?
        virtual glm::vec3 GetPlayerPositionInCurrentCube() = 0;
    //@}

};

#endif // __IGAMESTATE_PROJECTCUBE_HEADER
