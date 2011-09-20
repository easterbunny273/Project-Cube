#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <iostream>

class IEvent
{
public:
    IEvent() {}

    virtual void what() = 0;
};

class EventA : public IEvent
{
public:
    EventA() : IEvent() {}
    virtual void what() { std::cout << "event a" << m_iTest <<  std::endl; }

    int m_iTest;
};

class EventB : public IEvent
{
public:
    EventB() : IEvent() {}

    int m_iTest2;
    virtual void what() { std::cout << "event b" << m_iTest2*2 << std::endl; }
};

class TestClassUsedFromLua
{
public:
    TestClassUsedFromLua(int a) : m_a(a) {}
    void DoSomething() { std::cout << "testclass-dosomething called with " << m_a << std::endl; }
private:
    int m_a;
};

void TestMethodCalledFromLua(int a)
{
    std::cout << "c++ method called with a=" << a << std::endl;
}

EventA CreateEventA()
{
    return EventA();
}

EventB CreateEventB()
{
    return EventB();
}

void TestMethodClass(TestClassUsedFromLua &b)
{
    b.DoSomething();
}

void TestMethodEvent(IEvent *b)
{
    b->what();
}


TestClassUsedFromLua &TestMethodReturnClass(int a)
{
    static TestClassUsedFromLua test_instance(10);
    static TestClassUsedFromLua test_instance2(11);

    if (a < 10)
	return test_instance;
    else
	return test_instance2;
}

extern "C"
{
    void LuaTest()
    {
	// Create a new lua state
	lua_State *myLuaState = luaL_newstate();

	try
	{
	    // load lua libs (io, math, ...)
	    luaL_openlibs(myLuaState);

	    // Connect LuaBind to this lua state
	    luabind::open(myLuaState);

	    int iError = luaL_dofile(myLuaState, "scripts/test1.lua");
	    assert (iError == 0);

	    int iError2 = luaL_dofile(myLuaState, "scripts/test2.lua");
	    assert (iError2 == 0);

	    // Define a lua function that we can call
	    luaL_dostring(
	    myLuaState,
	    "function add(first, second)\n"
	    "  return first + second\n"
	    "end\n"
	    );

	    luabind::module(myLuaState) [
		luabind::def("TestMethodCalledFromLua", TestMethodCalledFromLua)
	    ];


	    luabind::module(myLuaState) [
		luabind::def("CreateEventA", CreateEventA),
		luabind::def("CreateEventB", CreateEventB)
	    ];

	    luabind::module(myLuaState) [
		luabind::def("TestMethodClass", TestMethodClass)
	    ];

	    luabind::module(myLuaState) [
		luabind::def("TestMethodEvent", TestMethodEvent)
	    ];

	    luabind::module(myLuaState) [
		luabind::def("TestMethodReturnClass", TestMethodReturnClass)
	    ];

	    luabind::module(myLuaState) [
		luabind::class_<TestClassUsedFromLua>("TestClass")
		.def(luabind::constructor<int>())
		.def("DoSomething", &TestClassUsedFromLua::DoSomething)
	    ];

	    luabind::module(myLuaState) [
		luabind::class_<IEvent>("IEvent"),
		luabind::class_<EventA, luabind::bases<IEvent> >("EventA")
		.def("what", &EventA::what)
		.def_readwrite("m_iTest", &EventA::m_iTest),
		luabind::class_<EventB, luabind::bases<IEvent> >("EventB")
		.def("what", &EventB::what)
		.def_readwrite("m_iTest2", &EventB::m_iTest2)
	    ];

	    std::cout << "Result: "
	    << luabind::call_function<int>(myLuaState, "add", 2, 3)
	    << std::endl;

	    luabind::call_function<void>(myLuaState, "start");
	    luabind::call_function<void>(myLuaState, "start2");
	}
	catch (luabind::error &e)
	{
	    std::cerr << e.what() << std::endl;
	    std::cerr << lua_tostring(myLuaState, -1) << std::endl;
	}

	lua_close(myLuaState);

    }
}

int main()
{
    int a=4;

    LuaTest();

   // TestMain::GetInstance()->Run();
   // MainApp::GetInstance()->Run();
}
