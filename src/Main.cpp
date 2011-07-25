#include "Settings.h"
#include "Logger.h"
#include "lua.hpp"
#include "ICube.h"

#include <iostream>

using namespace std;

int main()
{
    // some code to demonstrate the logger class

    Logger::SetLogLevelForConsole(Logger::ERROR);
    Logger::SetLogLevelForFile(Logger::DEBUG);

    Logger::debug() << "debug message test" << Logger::endl;
    Logger::error() << "error message test" << Logger::endl;

    int a = 3;

    Logger::error() << "this class can handle different types of variables. e.g: A=" << a << Logger::endl;

    /*Settings::instance()->GetGroup("graphics")->GetGroup("screen")->SetValue("width", 800);
    Settings::instance()->GetGroup("graphics")->GetGroup("screen")->SetValue("height", 600);
    Settings::instance()->GetGroup("audio")->SetValue("volume", 0.8f);*/
   // Settings::instance()->Clear();

    // for demonstration, read settings.xml and then write the settings to settings2.xml
    Settings::instance()->RestoreSettingsFromXMLFile("config/settings.xml");
    Settings::instance()->StoreSettingsAsXMLFile("config/settings2.xml");


	//code to demonstrate some LUA functionality
	lua_State * luastate = lua_open();
	luaL_openlibs(luastate);

	int s = luaL_loadfile(luastate, "lua/test.lua");

	if(s==0)
	{
		s = lua_pcall(luastate, 0, LUA_MULTRET, 0);
	}

	lua_close(luastate);
}
