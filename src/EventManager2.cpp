#include "EventManager2.h"

#include <iostream>
#include <sstream>


void EventManager::RegisterEventType(std::shared_ptr<IEvent> spEventPrototype)
{
    m_mPrototypes[spEventPrototype->GetName()] = spEventPrototype;
}

std::shared_ptr<IEvent> InputEvent::GetPrototype()
{
    std::shared_ptr<IEvent> spPrototype(new InputEvent());

    return spPrototype;
}

std::shared_ptr<IEvent> InputMouse::GetPrototype()
{
    std::shared_ptr<IEvent> spPrototype(new InputMouse());

    return spPrototype;
}

void EventManager::Test()
{
    RegisterEventType(std::shared_ptr<IEvent>(InputEvent::GetPrototype()));
    RegisterEventType(std::shared_ptr<IEvent>(InputMouse::GetPrototype()));

    std::shared_ptr<IEvent> spNewEvent = CreateEvent("input.key 3 2");
    std::shared_ptr<IEvent> spNewEvent1 = CreateEvent("input.mouse 3 2");

    std::cout << spNewEvent->GetName() << std::endl;
    std::cout << spNewEvent1->GetName() << std::endl;
}

std::shared_ptr<IEvent> EventManager::CreateEvent(std::string sCreateString)
{
    std::string sEventType;
    std::stringstream ssBuffer;

    ssBuffer << sCreateString;
    ssBuffer >> sEventType;

    if (m_mPrototypes.find(sEventType) != m_mPrototypes.end())
    {
	std::string sRestOfString = sCreateString.substr(sEventType.length()+1);

	return m_mPrototypes.find(sEventType)->second->CreateNewEventFromString(sRestOfString);
    }

    return std::shared_ptr<IEvent>();
}

InputEvent::InputEvent(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

std::shared_ptr<IEvent> InputEvent::CreateNewEventFromString(std::string sCreateString)
{
    int iX; int iY;
    std::stringstream ssBuffer;

    ssBuffer << sCreateString;
    ssBuffer >> iX >> iY;

    return std::shared_ptr<IEvent>(new InputEvent(iX, iY));
}

InputMouse::InputMouse(int x)
{
    m_iX = x;
}

std::shared_ptr<IEvent> InputMouse::CreateNewEventFromString(std::string sCreateString)
{
    int iX; int iY;
    std::stringstream ssBuffer;

    ssBuffer << sCreateString;
    ssBuffer >> iX;

    return std::shared_ptr<IEvent>(new InputMouse(iX));
}
