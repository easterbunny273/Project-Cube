#include "MainApp.h"
#include "Cube.h"
#include "Grid.h"

int main()
{
    MainApp::GetInstance()->Run();

	Grid grid1;
	Grid grid2;
	grid1.AddDoor(1,1);
	grid2.AddDoor(2,2);

	Cube cube;
	cube.SetGrids(grid1, grid2, grid1, grid2, grid1, grid2);
	
	cube.test();
}
