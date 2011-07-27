#include "MainApp.h"
#include "Grid.h"

#include <iostream>

using namespace std;

int main()
{
    MainApp::GetInstance()->Run();
	//test grid rotation

	Grid grid;
	grid.AddDoor(1,1);
	grid.AddDoor(1,1); //False
	grid.AddDoor(2,1); //False
	grid.AddDoor(0,1); //False
	grid.AddDoor(10,3);  //False
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




	
}
