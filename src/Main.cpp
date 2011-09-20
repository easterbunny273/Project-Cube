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

void TestMethodCalledFromLua(int a)
{
    std::cout << "c++ method called with a=" << a << std::endl;
}

extern "C"
{
    void LuaTest()
    {
	// Create a new lua state
	lua_State *myLuaState = luaL_newstate();

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

	std::cout << "Result: "
	<< luabind::call_function<int>(myLuaState, "add", 2, 3)
	<< std::endl;

	luabind::call_function<void>(myLuaState, "start");
	luabind::call_function<void>(myLuaState, "start2");

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
