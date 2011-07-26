#include "Settings.h"
#include "Logger.h"
#include "Graphics/Graphics.h"
#include "lua.hpp"
#include "ICube.h"
#include "Grid.h"

#include <iostream>

using namespace std;

int main()
{
    // some code to demonstrate the logger class

    Logger::SetLogLevelForConsole(Logger::ERROR);
    Logger::SetLogLevelForFile(Logger::DEBUG);

    Logger::debug() << "debug message test" << Logger::endl;
    Logger::error() << "error message test" << Logger::endl;

    int a = 3;

    Logger::error() << "this class can handle different types of variables. e.g: A=" << a << Logger::endl;

    /*Settings::instance()->GetGroup("graphics")->GetGroup("screen")->SetValue("width", 800);
    Settings::instance()->GetGroup("graphics")->GetGroup("screen")->SetValue("height", 600);
    Settings::instance()->GetGroup("audio")->SetValue("volume", 0.8f);*/
   // Settings::instance()->Clear();

    // for demonstration, read settings.xml and then write the settings to settings2.xml
    Settings::instance()->RestoreSettingsFromXMLFile("config/settings.xml");

	//test grid rotation

	Grid grid;
	grid.AddDoor(1,1);
	grid.AddDoor(4,4);
	grid.AddDoor(6,7);
	grid.AddDoor(1,5);
	grid.AddDoor(8,2);

	Logger::debug() << "initialized grid:" << Logger::endl;
	std::vector<glm::vec2> doors = grid.GetDoorPositions();
	for(int i = 0; i < doors.size(); i++)
	{
		Logger::debug() << "door"<< i << " x: " << doors.at(i).x << " y: " << doors.at(i).y << Logger::endl;
	}

	Logger::debug() << "rotate 1" << Logger::endl;
	grid.RotateGrid(1);
	doors = grid.GetDoorPositions();
	for(int i = 0; i < doors.size(); i++)
	{
		Logger::debug() << "door"<< i << " x: " << doors.at(i).x << " y: " << doors.at(i).y << Logger::endl;
	}

	Logger::debug() << "rotate 2" << Logger::endl;
	grid.RotateGrid(-1);
	grid.RotateGrid(2);
	doors = grid.GetDoorPositions();
	for(int i = 0; i < doors.size(); i++)
	{
		Logger::debug() << "door"<< i << " x: " << doors.at(i).x << " y: " << doors.at(i).y << Logger::endl;
	}

	Logger::debug() << "rotate 3" << Logger::endl;
	grid.RotateGrid(-2);
	grid.RotateGrid(3);
	doors = grid.GetDoorPositions();
	for(int i = 0; i < doors.size(); i++)
	{
		Logger::debug() << "door"<< i << " x: " << doors.at(i).x << " y: " << doors.at(i).y << Logger::endl;
	}

	Logger::debug() << "initialized grid:" << Logger::endl;
	grid.RotateGrid(-3);
	doors = grid.GetDoorPositions();
	for(int i = 0; i < doors.size(); i++)
	{
		Logger::debug() << "door"<< i << " x: " << doors.at(i).x << " y: " << doors.at(i).y << Logger::endl;
	}


    Graphics test;

    test.StartUp();

    for (int i=0; i < 100000000; i++)
    {
        // burn cpu time


        int *a = new int[10000];

        delete [] a;
    }

    test.ShutDown();

    Settings::instance()->StoreSettingsAsXMLFile("settings2.xml");


	
}
