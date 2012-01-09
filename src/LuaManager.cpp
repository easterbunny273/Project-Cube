#include "LuaManager.h"

LuaManager* LuaManager::instance = NULL;

LuaManager* LuaManager::GetInstance()
{
	if(instance == 0)
		instance = new LuaManager();
	return instance;
}

LuaManager::LuaManager()
{
	m_LuaState = lua_open();

	luaL_openlibs(m_LuaState);
}

void LuaManager::ExecuteFile(std::string sFile)
{
	luaL_dofile(m_LuaState, sFile.c_str());
}