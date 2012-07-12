#include "LuaManager.h"
#include "PC_Logger.h"
#include <iostream>

//includes for class registerings
#include "Gamelogic/Grid.h"
#include "Gamelogic/Cube.h"
#include "Gamelogic/Level.h"
#include "Gamelogic/Objects/LightObject.h"
#include "Gamelogic/Objects/Object.h"

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
	m_pLuaState = luaL_newstate();

	luaL_openlibs(m_pLuaState);

	luabind::open(m_pLuaState);

	ExecuteFile("lua/functions.lua");

	RegisterClasses();
}

lua_State* LuaManager::GetLuaState()
{
	return m_pLuaState;
}

void LuaManager::RegisterClasses()
{
	Logger::debug() << "Registering classes to LUA" << Logger::endl;
	// Register Grid class
	luabind::module(m_pLuaState)
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
	luabind::module(m_pLuaState)
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
	luabind::module(m_pLuaState)
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
				.def("CreateObject", &Level::CreateObject)
				.def("CreateLight", &Level::CreateLight)
				.def("GetObjectByName", &Level::GetObjectByName)
				.def("GetLightByName", &Level::GetLightByName)
		];

	// Register Object class
	luabind::module(m_pLuaState)
		[
			luabind::class_<Object>("Object")
				.def(luabind::constructor<std::string, std::string>())
				.def("Translate", &Object::Translate)
				.def("Scale", &Object::Scale)
				.def("ActivateEnvironmentMapping", &Object::ActivateEnvironmentMapping)
				.def("DeactivateEnvironmentMapping", &Object::DeactivateEnvironmentMapping)
				.def("GetName", &Object::GetName)
		];

	// Register Lightobject class
	luabind::module(m_pLuaState)
		[
			luabind::class_<LightObject>("Light")
				.def(luabind::constructor<std::string>())
				.def("GetObjectType", &LightObject::GetObjectType)
				.def("GetName", &LightObject::GetName)
				.def("SetPosition", &LightObject::SetPosition)
				.def("SetLookDirection", &LightObject::SetLookDirection)
				.def("SetFOV", &LightObject::SetFOV)
				.def("SetColor", &LightObject::SetColor)
				.def("SetNearplane", &LightObject::SetNearplane)
				.def("SetFarplane", &LightObject::SetFarplane)
		];
}

void LuaManager::ExecuteFile(std::string sFile)
{
	Logger::debug() << "Executing LUA file: "<< sFile << Logger::endl;
	try
	{
		luaL_dofile(m_pLuaState, sFile.c_str());
	}
	catch(std::exception e)
	{
		Logger::error() << e.what() << Logger::endl;
		lua_tostring(m_pLuaState, -1);
	}
}