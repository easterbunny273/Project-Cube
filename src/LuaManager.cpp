#include "LuaManager.h"
#include "PC_Logger.h"
#include <iostream>
#include "Gamelogic/Grid.h"

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

void PrintDebugMessageLuaManager(std::string sMessage)
{
	Logger::debug() << sMessage << Logger::endl;
}

void LuaManager::InitLua()
{
	m_LuaState = lua_open();

	luaL_openlibs(m_LuaState);

	luabind::open(m_LuaState);

	ExecuteFile("lua/functions.lua");

	luabind::module(m_LuaState)
		[
			luabind::def("PrintDebugMessageLuaManager", &PrintDebugMessageLuaManager)
		];

	luabind::module(m_LuaState)
		[
			luabind::class_<Grid>("Grid")
				.def(luabind::constructor<>())
				.def("AddDoor", (bool(Grid::*)(int, int))&Grid::AddDoor)
				.def("ClearGrid", &Grid::ClearGrid)
				.def("GetDoorPositions", &Grid::GetDoorPositions)
				.def("RotateGrid", &Grid::RotateGrid)
				.def("MirrorGridVertical", &Grid::MirrorGridVertical)
				.def("MirrorGridHorizontal", &Grid::MirrorGridHorizontal)
				.def("SetPosition", &Grid::SetPosition)
				.def("GetPosition", &Grid::GetPosition)
				.def("PrintGrid", &Grid::PrintGrid)
		];

}

void LuaManager::ExecuteFile(std::string sFile)
{
	luaL_dofile(m_LuaState, sFile.c_str());
}

