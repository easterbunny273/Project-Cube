#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "lua_include.h"

#include "EventManager.h"
#include "LuaManager.h"

#include "PC_Logger.h"

//include grid for testing registerclass
#include "Gamelogic/Grid.h"

void PrintDebugMessageMain(std::string sMessage)
{
	Logger::debug() << sMessage << Logger::endl;
}

int main()
{
	LuaManager::GetInstance()->RegisterFunction("PrintDebugMessageMain", &PrintDebugMessageMain);
	
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("ret") << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add1", 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add2", 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add3", 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add4", 1, 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add5", 1, 1, 1, 1, 2) << Logger::endl;
	
	LuaManager::GetInstance()->RegisterClass(luabind::class_<Grid>("Grid")
												.def(luabind::constructor<>())
												.def("AddDoor", (bool(Grid::*)(int, int))&Grid::AddDoor)
												.def("PrintGrid", &Grid::PrintGrid));

	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");
    //TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
