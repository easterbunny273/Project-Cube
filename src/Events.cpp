#include "Events.h"

#include <iostream>
#include <sstream>
#include <assert.h>

std::shared_ptr<EventManager::IEvent> InputKeyEvent::GetPrototype()
{
    std::shared_ptr<EventManager::IEvent> spPrototype(new InputKeyEvent());

    return spPrototype;
}

std::shared_ptr<EventManager::IEvent> InputKeyEvent::CreateNewEventFromString(std::string sCreateString)
{

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

void InputKeyEvent::RegisterLua()
{
}

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

std::shared_ptr<EventManager::IEvent> InputMouseButtonEvent::CreateNewEventFromString(std::string sCreateString)
{
    assert (!"not implemented yet");
}

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

std::shared_ptr<EventManager::IEvent> InputMouseMoveEvent::CreateNewEventFromString(std::string sCreateString)
{
    assert (!"not implemented yet");
}
