#include "CubeTest.h"
#include "Logger.h"

void CubeTest::setup()
{
	grid1.AddDoor(1,1);
	grid2.AddDoor(2,8);
	grid3.AddDoor(3,7);
	grid4.AddDoor(6,6);
	grid5.AddDoor(4,6);
	grid6.AddDoor(5,5);

	testcube.SetGrids(grid1, grid2, grid3, grid4, grid5, grid6);
}

void CubeTest::tear_down()
{
	grid1.ClearGrid();
	grid2.ClearGrid();
	grid3.ClearGrid();
	grid4.ClearGrid();
	grid5.ClearGrid();
	grid6.ClearGrid();

	testcube.SetGrids(grid1, grid2, grid3, grid4, grid5, grid6);
}

void CubeTest::TestCubeRotationX1()
{
	bool test = testcube.RotateX(1);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 1)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 9)
	
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 8)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 8)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 5)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 7)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 6)
}

void CubeTest::TestCubeRotationX2()
{
	bool test = testcube.RotateX(2);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 9)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 9)
	
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 8)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 2)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 3)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 5)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 4)
}

void CubeTest::TestCubeRotationX3()
{
	bool test = testcube.RotateX(3);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 9)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 1)
	
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 2)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 2)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 5)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 3)
}

void CubeTest::TestCubeRotationY1()
{
	bool test = testcube.RotateY(1);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 5)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 7)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 7)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 8)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 8)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 9)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 1)
}

void CubeTest::TestCubeRotationY2()
{
	bool test = testcube.RotateY(2);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 8)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 8)

	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 9)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 1)

	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 7)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 3)

	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 4)

	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 5)

	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 6)
}

void CubeTest::TestCubeRotationY3()
{
	bool test = testcube.RotateY(3);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 5)

	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 3)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 1)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 1)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 2)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 8)
}

void CubeTest::TestCubeRotationZ1()
{
	bool test = testcube.RotateZ(1);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 7)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 1)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 9)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 8)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 8)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 5)
}

void CubeTest::TestCubeRotationZ2()
{
	bool test = testcube.RotateZ(2);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 2)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 2)

	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 1)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 9)

	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 6)

	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 7)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 7)

	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 6)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 4)

	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 5)
}

void CubeTest::TestCubeRotationZ3()
{
	bool test = testcube.RotateZ(3);
	TEST_ASSERT(test)

	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).x == 3)
	TEST_ASSERT(testcube.GetGrid(1).GetDoorPositions().at(0).y == 3)

	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(2).GetDoorPositions().at(0).y == 6)
	
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).x == 2)
	TEST_ASSERT(testcube.GetGrid(3).GetDoorPositions().at(0).y == 8)
	
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).x == 9)
	TEST_ASSERT(testcube.GetGrid(4).GetDoorPositions().at(0).y == 9)
	
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).x == 4)
	TEST_ASSERT(testcube.GetGrid(5).GetDoorPositions().at(0).y == 4)
	
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).x == 5)
	TEST_ASSERT(testcube.GetGrid(6).GetDoorPositions().at(0).y == 5)
}