#include "EventManager.h"
#include "Events.h"
#include "Logger.h"

#include <iostream>
#include <sstream>
#include <list>

#include <assert.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
}

#include <luabind/luabind.hpp>

EventManager::EventManager()
{
    // register all known event prototypes
    RegisterEvent<InputKeyEvent>();
    RegisterEvent<InputMouseButtonEvent>();
    RegisterEvent<InputMouseMoveEvent>();
}

void EventManager::ItlRegisterEventPrototype(std::shared_ptr<IEvent> spEventPrototype)
{
    for (auto iter : m_lPrototypes)
    {
	if ((*iter).GetName() == spEventPrototype->GetName())
	    Logger::fatal() << "You tried to register an already registered event type" << Logger::endl;
    }

    m_lPrototypes.push_back(spEventPrototype);
}

void EventManager::Test()
{
  /*  std::shared_ptr<IEvent> spNewEvent = CreateEvent("input.key 3 2");
    std::shared_ptr<IEvent> spNewEvent1 = CreateEvent("input.mouse 3 2");

    std::cout << spNewEvent->GetName() << std::endl;
    std::cout << spNewEvent1->GetName() << std::endl;

    // Create a new lua state
    lua_State *myLuaState = lua_open();

    // Connect LuaBind to this lua state
    luabind::open(myLuaState);


    luabind::module(myLuaState)
    [
	luabind::class_<EventManager>("EventManager")
	    .def("CreateEvent2", &EventManager::CreateEvent2),
	luabind::class_<IEvent>("IEvent"),
	luabind::class_<InputKeyEvent>("InputEvent"),
	luabind::class_<InputMouse>("InputMouse")
	//luabind::def("eventmanager", this)
    ];

    luaL_openlibs(myLuaState);

    luaL_dostring(
    myLuaState,
    "function print2()\n"
    "  print \"hallo\""
    "end\n"
    );

    luabind::globals(myLuaState)["eventmanager2"] = this;

    // Define a lua function that we can call
    luaL_dostring(
    myLuaState,
    "function test2()\n"
    "  print2()\n"
    "  eventmanager2:CreateEvent2(\"input.key 105 109\")\n"
    "  print2()\n"
    "  return 4;\n"
    "end\n"
    );

    try
    {
	int a = luabind::call_function<int>(myLuaState, "test2", "");

	std::cout << a << std::endl;
    }
    catch (luabind::error &e)
    {
	std::cerr << lua_tostring(myLuaState, -1) << std::endl;
    }

    lua_close(myLuaState);*/
}

std::shared_ptr<EventManager::IEvent> EventManager::CreateEvent(const char * szCreateString)
{
    std::string sCreateString(szCreateString);
    std::string sEventType;
    std::stringstream ssBuffer;

    ssBuffer << sCreateString;
    ssBuffer >> sEventType;

    std::string sRestOfString = sCreateString.substr(sEventType.length()+1);

    for (auto iter=m_lPrototypes.begin(); iter != m_lPrototypes.end(); iter++)
    {
	return (*iter)->CreateNewEventFromString(sRestOfString);
    }

    return std::shared_ptr<EventManager::IEvent>();
}





void EventManager::CreateEvent2(const char *szCreateString)
{
    CreateEvent(szCreateString);
}

void EventManager::RegisterEventListener(EventManager::IEventListener *pListener,
					 std::vector<std::string> vsEventNames)
{
    for (auto iter = vsEventNames.begin(); iter != vsEventNames.end(); iter++)
    {
	std::list<IEventListener*> *plListenerForEvent = &(m_mEventListener[*iter]);
	plListenerForEvent->push_back(pListener);
    }
}

void EventManager::RegisterEventListener(IEventListener *pListener, std::string sEventName)
{
    m_mEventListener[sEventName].push_back(pListener);
}

void EventManager::TriggerEvent(std::shared_ptr<IEvent> spEvent)
{
    assert (ItlCheckIfEventIsRegistered(spEvent));

    std::list<IEventListener*> *plListenerForEvent = &(m_mEventListener[spEvent->GetName()]);

    for (auto iter=plListenerForEvent->begin(); iter != plListenerForEvent->end(); iter++)
    {
	IEventListener *pListener = *iter;

	pListener->OnEvent(spEvent);
    }
}

void EventManager::QueueEvent(std::shared_ptr<IEvent> spEvent)
{
    assert(m_nActiveQueue < 2);

    unsigned int nInactiveQueue = (m_nActiveQueue==1) ? 0 : 1;

    assert (m_nActiveQueue != nInactiveQueue);
    assert (nInactiveQueue < 2);

    m_lEventQueue[nInactiveQueue].push_back(spEvent);
}

void EventManager::ProcessEvents()
{
    assert (m_nActiveQueue < 2);

    for (auto iter=m_lEventQueue[m_nActiveQueue].begin(); iter != m_lEventQueue[m_nActiveQueue].end(); iter++)
    {
	std::shared_ptr<IEvent> *pspCurrentEvent = &(*iter);

	TriggerEvent(*pspCurrentEvent);
    }

    // clear queue
    m_lEventQueue[m_nActiveQueue].clear();

    // swap active queue
    m_nActiveQueue = (m_nActiveQueue==1) ? 0 : 1;

    assert (m_nActiveQueue < 2);
}

bool EventManager::ItlCheckIfEventIsRegistered(std::shared_ptr<IEvent> spEvent)
{
    bool bFound = false;

    for (auto iter=m_lPrototypes.begin(); iter != m_lPrototypes.end(); iter++)
    {
	if ((*iter)->GetName() == spEvent->GetName())
	    bFound = true;
    }

    return bFound;
}
