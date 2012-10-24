/*
 * source file for EventManager class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 09/2011
 */

// project includes
#include "EventManager.h"
#include "Events.h"
#include "BambooLib/include/Logger.h"

// stl includes
#include <iostream>
#include <sstream>
#include <list>

// use C-style assert
#include <assert.h>


// static member initialisation
int EventManager::IEvent::s_iCount = 0;

/****************************************************************
  *************************************************************** */
EventManager::EventManager()
{
	m_nActiveQueue = 0;
}

/****************************************************************
  *************************************************************** */
EventManager::~EventManager()
{
    // clear prototypes
    m_lPrototypes.clear();

    // clear unprocessed events
    m_lEventQueue[0].clear();
    m_lEventQueue[1].clear();

    // now, all created events should be deleted because we are using shared_ptr
    // and no references exist any more
    int iEventsLeft = EventManager::IEvent::s_iCount;
    assert (iEventsLeft == 0);
}

/****************************************************************
  *************************************************************** */
void EventManager::ItlRegisterEventPrototype(std::shared_ptr<IEvent> spEventPrototype)
{
	for (auto iter = m_lPrototypes.begin(); iter != m_lPrototypes.end(); iter++)
    {
		if ((*iter)->GetEventType() == spEventPrototype->GetEventType())
			BambooLib::Logger::fatal() << "You tried to register an already registered event type" << BambooLib::Logger::endl;
    }
    /*for (auto iter : m_lPrototypes)
    {
	if ((*iter).GetEventType() == spEventPrototype->GetEventType())
	    Logger::fatal() << "You tried to register an already registered event type" << Logger::endl;
    }*/

    m_lPrototypes.push_back(spEventPrototype);
}


/****************************************************************
  *************************************************************** */
void EventManager::RegisterEventListener(EventManager::IEventListener *pListener,
					 std::vector<IEvent::TEventType> vtEventTypes)
{
    for (auto iter = vtEventTypes.begin(); iter != vtEventTypes.end(); iter++)
    {
	RegisterEventListener(pListener, *iter);
    }
}

/****************************************************************
  *************************************************************** */
void EventManager::RegisterEventListener(IEventListener *pListener,
					 IEvent::TEventType tEventType)
{
    // check if this concrete listener has already registered for that event type

    std::list<IEventListener*> *plListenerForEvent = &(m_mEventListener[tEventType]);

	for (auto iter = plListenerForEvent->begin(); iter != plListenerForEvent->end(); iter++)
    //for (auto iter : *plListenerForEvent)
    {
		IEventListener *pIterListener = (*iter);

		if (pIterListener == pListener)
		{
			BambooLib::Logger::fatal() << "The listener has already registered for that event type" << BambooLib::Logger::endl;
		}
    }


    m_mEventListener[tEventType].push_back(pListener);
}

/****************************************************************
  *************************************************************** */
void EventManager::TriggerEvent(std::shared_ptr<IEvent> spEvent)
{
    static int iNestedCalls = 0;
    bool bIsOuterLoop = (iNestedCalls == 0);

    iNestedCalls++;

    assert(iNestedCalls < 100);

    assert (ItlCheckIfEventIsRegistered(spEvent));

    std::list<IEventListener*> *plListenerForEvent = &(m_mEventListener[spEvent->GetEventType()]);

    for (auto iter=plListenerForEvent->begin(); iter != plListenerForEvent->end(); iter++)
    {
	IEventListener *pListener = *iter;

	pListener->OnEvent(spEvent);
    }

    iNestedCalls--;

    assert (!bIsOuterLoop || iNestedCalls==0);
}

/****************************************************************
  *************************************************************** */
void EventManager::QueueEvent(std::shared_ptr<IEvent> spEvent)
{
    assert(m_nActiveQueue < 2);

    m_lEventQueue[m_nActiveQueue].push_back(spEvent);
}

/****************************************************************
  *************************************************************** */
void EventManager::ProcessEvents()
{
    assert (m_nActiveQueue < 2);

    unsigned int nLastActiveQueue = m_nActiveQueue;

    // swap active queue
    m_nActiveQueue = (m_nActiveQueue==1) ? 0 : 1;

    assert (m_nActiveQueue != nLastActiveQueue);
    assert (m_nActiveQueue < 2);

    for (auto iter=m_lEventQueue[nLastActiveQueue].begin(); iter != m_lEventQueue[nLastActiveQueue].end(); iter++)
    {
	std::shared_ptr<IEvent> *pspCurrentEvent = &(*iter);

	TriggerEvent(*pspCurrentEvent);
    }

    // clear processed queue
    m_lEventQueue[nLastActiveQueue].clear();
}

/****************************************************************
  *************************************************************** */
bool EventManager::ItlCheckIfEventIsRegistered(std::shared_ptr<IEvent> spEvent)
{
    bool bFound = false;

    for (auto iter=m_lPrototypes.begin(); iter != m_lPrototypes.end(); iter++)
    {
	if ((*iter)->GetEventType() == spEvent->GetEventType())
	    bFound = true;
    }

    return bFound;
}


void EventManager::Initialize()
{
    // register all known event prototypes
    ItlRegisterEvent<InputKeyEvent>();
    ItlRegisterEvent<InputMouseButtonEvent>();
    ItlRegisterEvent<InputMouseMoveEvent>();
    ItlRegisterEvent<CameraMovementEvent>();

    // register itself in lua environment
    //RegisterLua();
}
