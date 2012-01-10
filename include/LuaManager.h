#ifndef __PROJECT_CUBE_LUA_MANAGER_HEADER
#define __PROJECT_CUBE_LUA_MANAGER_HEADER

#include "lua_include.h"
#include <string>

class LuaManager
{
public:
	static LuaManager* GetInstance();
	
	void ExecuteFile(std::string sFile);
	
	template <class F>
	void RegisterFunction(const char* name, F f);

private:
	static LuaManager* instance;

	lua_State* m_LuaState;

	LuaManager();
	~LuaManager();
	LuaManager(const LuaManager&);

	void InitLua();

};

template <class F>
void LuaManager::RegisterFunction(const char* name, F f)
{
	Logger::debug() << "Registering function in the LUA environment: " << name << Logger::endl;

	luabind::module(m_LuaState)
		[
			luabind::def(name, f)
		];
}

#endif //__PROJECT_CUBE_LUA_MANAGER_HEADER