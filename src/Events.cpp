#include "Events.h"

#include <iostream>
#include <sstream>
#include <assert.h>


EventManager::IEvent::TEventType InputKeyEvent::s_szEventType		    = "input.key";
EventManager::IEvent::TEventType InputMouseButtonEvent::s_szEventType	    = "input.mouse.button";
EventManager::IEvent::TEventType InputMouseMoveEvent::s_szEventType	    = "input.mouse.move";
EventManager::IEvent::TEventType CameraMovementEvent::s_szEventType	    = "camera.move";

/*
 * *************************************
 * InputKeyEvent
 * *************************************
 */

std::shared_ptr<EventManager::IEvent> InputKeyEvent::GetPrototype()
{
    std::shared_ptr<EventManager::IEvent> spPrototype(new InputKeyEvent());

    return spPrototype;
}

std::shared_ptr<EventManager::IEvent> InputKeyEvent::Create(TKey eKey, TEvent eEvent)
{
    InputKeyEvent *pEvent = new InputKeyEvent();

    pEvent->m_eKey = eKey;
    pEvent->m_eEvent = eEvent;

    return std::shared_ptr<EventManager::IEvent>(pEvent);
}

std::shared_ptr<EventManager::IEvent> InputKeyEvent::CreateFromString(std::string sParameters)
{
    assert (!"not implemented yet");

/*    int iX; int iY;
    std::stringstream ssBuffer;

    ssBuffer << sParameters;
    ssBuffer >> iX >> iY;

    return Create(iX, iY);*/
    return std::shared_ptr<EventManager::IEvent>();
}


std::shared_ptr<InputKeyEvent> InputKeyEvent::Cast(std::shared_ptr<EventManager::IEvent> spEvent)
{
    return std::dynamic_pointer_cast<InputKeyEvent>(spEvent);
}

/*
 * *************************************
 * InputMouseButtonEvent
 * *************************************
 */
std::shared_ptr<EventManager::IEvent> InputMouseButtonEvent::Create(InputMouseButtonEvent::TMouseButton eButton,
								    InputMouseButtonEvent::TEvent eEvent)
{
    InputMouseButtonEvent *pEvent = new InputMouseButtonEvent();

    pEvent->m_eButton = eButton;
    pEvent->m_eEvent = eEvent;

    return std::shared_ptr<EventManager::IEvent>(pEvent);
}

std::shared_ptr<EventManager::IEvent> InputMouseButtonEvent::GetPrototype()
{
    std::shared_ptr<EventManager::IEvent> spPrototype(new InputMouseButtonEvent());

    return spPrototype;
}

std::shared_ptr<InputMouseButtonEvent> InputMouseButtonEvent::Cast(std::shared_ptr<EventManager::IEvent> spEvent)
{
    return std::dynamic_pointer_cast<InputMouseButtonEvent>(spEvent);
}

/*
 * *************************************
 * InputMouseMoveEvent
 * *************************************
 */
std::shared_ptr<EventManager::IEvent> InputMouseMoveEvent::GetPrototype()
{
    std::shared_ptr<EventManager::IEvent> spPrototype(new InputMouseMoveEvent());

    return spPrototype;
}

std::shared_ptr<EventManager::IEvent> InputMouseMoveEvent::Create(int iX, int iY)
{
    InputMouseMoveEvent *pEvent = new InputMouseMoveEvent();

    pEvent->m_iX = iX;
    pEvent->m_iY = iY;

    return std::shared_ptr<EventManager::IEvent>(pEvent);
}

std::shared_ptr<InputMouseMoveEvent> InputMouseMoveEvent::Cast(std::shared_ptr<EventManager::IEvent> spEvent)
{
    return std::dynamic_pointer_cast<InputMouseMoveEvent>(spEvent);
}


/*
 * *************************************
 * CameraMovementEvent
 * *************************************
 */
std::shared_ptr<EventManager::IEvent> CameraMovementEvent::GetPrototype()
{
    std::shared_ptr<EventManager::IEvent> spPrototype(new CameraMovementEvent());

    return spPrototype;
}

std::shared_ptr<EventManager::IEvent> CameraMovementEvent::Create(TMovementType tMovementType, float fValue)
{
    CameraMovementEvent *pEvent = new CameraMovementEvent();

    pEvent->m_eMovementType = tMovementType;
    pEvent->m_fValue = fValue;

    return std::shared_ptr<EventManager::IEvent>(pEvent);
}

std::shared_ptr<CameraMovementEvent> CameraMovementEvent::Cast(std::shared_ptr<EventManager::IEvent> spEvent)
{
    return std::dynamic_pointer_cast<CameraMovementEvent>(spEvent);
}

