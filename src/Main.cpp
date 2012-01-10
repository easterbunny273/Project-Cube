#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "lua_include.h"

#include "EventManager.h"
#include "LuaManager.h"


#include "PC_Logger.h"

void PrintDebugMessageMain(std::string sMessage)
{
	Logger::debug() << sMessage << Logger::endl;
}

int main()
{
	LuaManager::GetInstance()->RegisterFunction("PrintDebugMessageMain", &PrintDebugMessageMain);
	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");
    //TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
