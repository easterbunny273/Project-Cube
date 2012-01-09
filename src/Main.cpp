#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "EventManager.h"
#include "LuaManager.h"

#include "lua.hpp"
#include <luabind/luabind.hpp>

int main()
{
	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");
    //TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
