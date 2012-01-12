#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "lua_include.h"

#include "EventManager.h"
#include "LuaManager.h"

#include "PC_Logger.h"

//include grid for testing registerclass
#include "Gamelogic/Grid.h"

int main()
{
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("ret") << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add1", 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add2", 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add3", 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add4", 1, 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add5", 1, 1, 1, 1, 2) << Logger::endl;

	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");

    //TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
