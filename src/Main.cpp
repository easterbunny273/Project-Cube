#include "MainApp.h"
#include "TestMain.h"
#include "Gamelogic/Level.h"

#include "lua_include.h"

#include "EventManager.h"
#include "LuaManager.h"

#include "BambooLib/include/Logger.h"

using namespace BambooLib;

int main()
{
	/*
	//	LUA TESTS
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("ret") << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add1", 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add2", 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add3", 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add4", 1, 1, 1, 2) << Logger::endl;
	Logger::debug() << LuaManager::GetInstance()->CallLuaFunction<int>("add5", 1, 1, 1, 1, 2) << Logger::endl;

	LuaManager::GetInstance()->ExecuteFile("lua/test.lua");

	Level level = LuaManager::GetInstance()->CallLuaFunction<Level>("GetLevel");
	level.GetCubeByPosition(0,0,0)->GetGrid(3).PrintGrid();
	*/

	
	// grid vertices calculation test
	/*Grid grid;
	grid.AddDoor(5,2);
	grid.AddDoor(5,9);
	grid.AddDoor(3,3);
	
	//grid.AddDoor(8,6);
	//grid.AddDoor(1,7);

	std::vector<glm::ivec2> vertices = grid.CalcVertices();

	int trianglecount = 1;
	int i = 1;
	while(vertices.size() != 0)
	{
		glm::ivec2 temp = vertices.back();
		Logger::debug() << "Triangle " << trianglecount << ": (" << temp.x << "|" << temp.y << ")" << Logger::endl;
		vertices.pop_back();
		if(i == 3)
		{
			i = 0;
			trianglecount++;
			Logger::debug() << "------------------------------------" << Logger::endl;
		}
		i++;
	}
	*/

    //TestMain::GetInstance()->Run();
    Logger::ILogWriter *pLogWriterConsole = Logger::ConsoleLogWriter::Create();
    Logger::ILogWriter *pLogWriterHTML = Logger::HTMLLogWriter::Create("log.html");

    pLogWriterConsole->SetIgnoreBelow(Logger::DEBUG);
    pLogWriterHTML->SetIgnoreBelow(Logger::DEBUG);

    Logger::RegisterLogWriter(pLogWriterConsole);
    Logger::RegisterLogWriter(pLogWriterHTML);

    MainApp::GetInstance()->Run();

    delete pLogWriterConsole;
    delete pLogWriterHTML;
}
