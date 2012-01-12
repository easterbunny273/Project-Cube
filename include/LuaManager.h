#ifndef __PROJECT_CUBE_LUA_MANAGER_HEADER
#define __PROJECT_CUBE_LUA_MANAGER_HEADER


#include "lua_include.h"
#include <string>
#include <cstdarg>



class LuaManager
{
public:
	static LuaManager* GetInstance();
	
	void ExecuteFile(std::string sFile);
	
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

	template <class A>
	void RegisterClass(luabind::class_<A> c);


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

template <class R>
R LuaManager::CallLuaFunction(const char* name)
{
	return luabind::call_function<R>(m_LuaState, name);
}

template <class R, class A>
R LuaManager::CallLuaFunction(const char* name, A a)
{
	return luabind::call_function<R>(m_LuaState, name, a);
}

template <class R, class A, class B>
R LuaManager::CallLuaFunction(const char* name, A a, B b)
{
	return luabind::call_function<R>(m_LuaState, name, a, b);
}

template <class R, class A, class B, class C>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c)
{
	return luabind::call_function<R>(m_LuaState, name, a, b, c);
}

template <class R, class A, class B, class C, class D>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c, D d)
{
	return luabind::call_function<R>(m_LuaState, name, a, b, c, d);
}

template <class R, class A, class B, class C, class D, class E>
R LuaManager::CallLuaFunction(const char* name, A a, B b, C c, D d, E e)
{
	return luabind::call_function<R>(m_LuaState, name, a, b, c, d, e);
}

template <class A>
void LuaManager::RegisterClass(luabind::class_<A> classDescriptor)
{
	luabind::module(m_LuaState)
		[
			classDescriptor
		];
}

#endif //__PROJECT_CUBE_LUA_MANAGER_HEADER