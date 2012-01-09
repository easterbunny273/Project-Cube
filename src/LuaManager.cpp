#include "LuaManager.h"
#include "PC_Logger.h"
#include <iostream>
#include <luabind/luabind.hpp>

LuaManager* LuaManager::instance = NULL;

LuaManager* LuaManager::GetInstance()
{
	if(instance == 0)
	{
		instance = new LuaManager();
		instance->InitLua();
	}
	return instance;
}

LuaManager::LuaManager()
{
}

void PrintDebugMessage(std::string sMessage)
{
	Logger::debug() << sMessage << Logger::endl;
}

void LuaManager::InitLua()
{
	m_LuaState = lua_open();

	luaL_openlibs(m_LuaState);

	luabind::open(m_LuaState);

	luabind::module(m_LuaState)
		[
			luabind::def("PrintDebugMessage", &PrintDebugMessage)
		];
}

void LuaManager::ExecuteFile(std::string sFile)
{
	luaL_dofile(m_LuaState, sFile.c_str());
	
}

