/*
 * header file for DummyGame class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 07/2011, Projekt CUBE
 */

#ifndef __HEADER_DUMMYGAME
#define __HEADER_DUMMYGAME

#include "Gamelogic/IGameState.h"
#include "Gamelogic/IObject.h"
#include "IInputEventListener.h"


#include <list>
#include <memory>

#include <glm/gtc/type_ptr.hpp>

/**
  *  This class is just a dummy class to provide some objects
  *  to the graphics engine.
  *
  *
  */

class DummyGame : public IGameState, public IInputEventListener
{
public:
    /*! \name IGameState interface */
    //@{
	/// returns a list of the objects in the current cube
	virtual std::list<IObject *> GetObjectsInCube();

	/// returns a shared_ptr to the currently active cube
	/// todo: is a shared_ptr necessary?
	virtual std::shared_ptr<ICube *> GetCurrentCube();

	/// returns the position of the player in the current cube
	/// todo: what's the reference point?
	virtual glm::vec3 GetPlayerPositionInCurrentCube();
    //@}

    /*! \name Public signals for key handling */
    //@{
	/// fired when a key is down
	virtual void OnKeyDown(TKey eKey);

	/// fired when a key goes up
	virtual void OnKeyUp(TKey eKey);
    //@}

    /*! \name Public signals for mouse handling */
    //@{
	/// fired when a key is pressed
	virtual void OnMouseMove(int iX, int iY);

	/// fired when a mouse button is pressed
	virtual void OnMouseButtonPressed(TMouseButton eButton);

	/// fired when a mouse button is released
	virtual void OnMouseButtonReleased(TMouseButton eButton);
    //@}
};

#endif
