#ifndef __PROJECT_CUBE_LUA_MANAGER_HEADER
#define __PROJECT_CUBE_LUA_MANAGER_HEADER


#include "lua_include.h"
#include <string>
#include <cstdarg>

#include "PC_Logger.h"


class LuaManager
{
public:
	static LuaManager* GetInstance();
	
	void ExecuteFile(std::string sFile);
	lua_State* GetLuaState();

	template <class F>
	void RegisterFunction(const char* name, F f);
	
	template <class R>
	R CallLuaFunction(const char* name);
	template <class R, class A>
	R CallLuaFunction(const char* name, A a);
	template <class R, class A, class B>
	R CallLuaFunction(const char* name, A a, B b);
	template <class R, class A, class B, class C>
	R CallLuaFunction(const char* name, A a, B b, C c);
	template <class R, class A, class B, class C, class D>
	R CallLuaFunction(const char* name, A a, B b, C c, D d);
	template <class R, class A, class B, class C, class D, class E>
	R CallLuaFunction(const char* name, A a, B b, C c, D d, E e);

private:
	static LuaManager* instance;

	lua_State* m_pLuaState;

	LuaManager();
	~LuaManager();
	LuaManager(const LuaManager&);

	void InitLua();
	void RegisterClasses();

};

template <class F>
void LuaManager::RegisterFunction(const char* name, F f)
{
	Logger::debug() << "Registering function in the LUA environment: " << name << Logger::endl;

	luaponte::module(m_pLuaState)
		[
			luaponte::def(name, f)
		];
}

template <class R>
R LuaManager::CallLuaFunction(const char* name)
{
	return luaponte::call_function<R>(m_pLuaState, name);
}

template <class R, class A>
R LuaManager::CallLuaFunction(const char* name, A a)
{
	return luaponte::call_function<R>(m_pLuaState, name, a);
}

template <class R, class A, class B>
R LuaManager::CallLuaFunction(const char* name, A a, B b)
{
	return luaponte::call_function<R>(m_pLuaState, name, a, b);
}

template <class R, class A, class B, class C>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c)
{
	return luaponte::call_function<R>(m_pLuaState, name, a, b, c);
}

template <class R, class A, class B, class C, class D>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c, D d)
{
	return luaponte::call_function<R>(m_pLuaState, name, a, b, c, d);
}

template <class R, class A, class B, class C, class D, class E>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c, D d, E e)
{
	return luaponte::call_function<R>(m_pLuaState, name, a, b, c, d, e);
}

#endif //__PROJECT_CUBE_LUA_MANAGER_HEADER
