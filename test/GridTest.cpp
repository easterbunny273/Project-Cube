#include "GridTest.h"


void GridTest::setup()
{
	grid1.AddDoor(2,2);
	grid1.AddDoor(1,8);
	grid1.AddDoor(7,5);

	grid2.AddDoor(5,5);
	grid2.AddDoor(9,9);
	grid2.AddDoor(1,2);
	grid2.AddDoor(8,6);
}

void GridTest::tear_down()
{
	grid1.ClearGrid();
	grid2.ClearGrid();
}

void GridTest::TestDoorInsertion()
{
	TEST_ASSERT(grid1.AddDoor(4,4));
	TEST_ASSERT(grid1.AddDoor(2,2)==false);
	TEST_ASSERT(grid1.AddDoor(7,6)==false);
	TEST_ASSERT(grid1.AddDoor(3,3)==false);
	TEST_ASSERT(grid1.AddDoor(0,9)==false);
	TEST_ASSERT(grid1.AddDoor(5,10)==false);
	TEST_ASSERT(grid1.AddDoor(100, -100)==false);
}

void GridTest::TestRotation1()
{
}

void GridTest::TestRotation2()
{
}

void GridTest::TestRotation3()
{
}