#include "LuaManager.h"
#include "PC_Logger.h"
#include <iostream>

//includes for class registerings
#include "Gamelogic/Grid.h"
#include "Gamelogic/Cube.h"
#include "Gamelogic/Level.h"

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

void LuaManager::InitLua()
{
	m_LuaState = luaL_newstate();

	luaL_openlibs(m_LuaState);

	luabind::open(m_LuaState);

	ExecuteFile("lua/functions.lua");

	RegisterClasses();
}

void LuaManager::RegisterClasses()
{
	// Register Grid class
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
	
	// Register Cube class
	luabind::module(m_LuaState)
		[
			luabind::class_<Cube>("Cube")
				.def(luabind::constructor<>())
				.def("GetCubeID", &Cube::GetCubeID)
				.def("GetCubePosition", &Cube::GetCubePosition)
				.def("GetTransfomation", &Cube::GetTransformation)
				.def("RotateX", &Cube::RotateX)
				.def("RotateY", &Cube::RotateY)
				.def("RotateZ", &Cube::RotateZ)
				.def("SetGrids", &Cube::SetGrids)
				.def("SetXplus", &Cube::SetXplus)
				.def("SetXminus", &Cube::SetXminus)
				.def("SetYplus", &Cube::SetYplus)
				.def("SetYminus", &Cube::SetYminus)
				.def("SetZplus", &Cube::SetZplus)
				.def("SetZminus", &Cube::SetZminus)
				.def("GetGrid", &Cube::GetGrid)
				.def("GetX", &Cube::GetX)
				.def("GetY", &Cube::GetY)
				.def("GetZ", &Cube::GetZ)
				.def("SetX", &Cube::SetX)
				.def("SetY", &Cube::SetY)
				.def("SetZ", &Cube::SetZ)
		];

	// Register Level class
	luabind::module(m_LuaState)
		[
			luabind::class_<Level>("Level")
				.def(luabind::constructor<>())
				.def("GetLevelID", &Level::GetLevelID)
				.def("GetLevelName", &Level::GetLevelName)
				.def("GetNumCubes", &Level::GetNumCubes)
				.def("Clear", &Level::Clear)
				.def("RotateX", &Level::RotateX)
				.def("RotateY", &Level::RotateY)
				.def("RotateZ", &Level::RotateZ)
				.def("StoreLevelAsXMLFile", &Level::StoreLevelAsXMLFile)
				.def("LoadLevelFromXMLFile", &Level::LoadLevelFromXMLFile)
				.def("GetCubes", &Level::GetCubes)
				.def("GetCubeByPosition", (Cube*(Level::*)(int, int, int))&Level::GetCubeByPosition)
		];
}

void LuaManager::ExecuteFile(std::string sFile)
{
	luaL_dofile(m_LuaState, sFile.c_str());
}

