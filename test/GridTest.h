#include "cpptest.h"
using namespace std;

#include "Grid.h"

class GridTest : public Test::Suite {

public:
	GridTest()
	{
		TEST_ADD(GridTest::TestDoorInsertion);
		TEST_ADD(GridTest::TestRotation1);
		TEST_ADD(GridTest::TestRotation2);
		TEST_ADD(GridTest::TestRotation3);
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Grid grid1;
	Grid grid2;

	void TestDoorInsertion();
	void TestRotation1();
	void TestRotation2();
	void TestRotation3();
};