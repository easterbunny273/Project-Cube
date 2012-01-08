#include "LuaManager.h"

LuaManager* LuaManager::instance = NULL;

LuaManager* LuaManager::GetInstance()
{
	if(instance = 0)
		instance = new LuaManager();
	return instance;
}