#include "lua.hpp"
#include <string>

class LuaManager
{
public:
	static LuaManager* GetInstance();
	
	void ExecuteFile(std::string sFile);
private:
	static LuaManager* instance;

	lua_State* m_LuaState;

	LuaManager();
	~LuaManager();
	LuaManager(const LuaManager&);
};