#include "EventManager2.h"

#include <iostream>
#include <sstream>

extern "C"
{
#include <lua.h>
#include <lualib.h>
}

#include <luabind/luabind.hpp>

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

    // Create a new lua state
    lua_State *myLuaState = lua_open();

    // Connect LuaBind to this lua state
    luabind::open(myLuaState);


    luabind::module(myLuaState)
    [
	luabind::class_<EventManager>("EventManager")
	    .def("CreateEvent2", &EventManager::CreateEvent2),
	luabind::class_<IEvent>("IEvent"),
	luabind::class_<InputEvent>("InputEvent"),
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
	/*luaL_dostring(
	myLuaState,
	"test2()\n"
	);*/
    }
    catch (luabind::error &e)
    {
	std::cerr << lua_tostring(myLuaState, -1) << std::endl;
    }

    lua_close(myLuaState);
}

std::shared_ptr<IEvent> EventManager::CreateEvent(const char * szCreateString)
{
    std::string sCreateString(szCreateString);
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

    std::cout << "created input event " << x << ":" << y << std::endl;
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

EventManager::EventManager()
{
    std::cout << "created an event manager" << std::endl;
}

void EventManager::CreateEvent2(const char *szCreateString)
{
    CreateEvent(szCreateString);
}
