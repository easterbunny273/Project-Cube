#include "lua_include.h"

class LuaManager
{
public:
	static LuaManager* GetInstance();
	
private:
	static LuaManager* instance;

	LuaManager();
	~LuaManager();
	LuaManager(const LuaManager&);
};