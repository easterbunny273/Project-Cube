#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "EventManager.h"
#include "LuaManager.h"

int main()
{
	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");
    //TestMain::GetInstance()->Run();
    MainApp::GetInstance()->Run();
}
