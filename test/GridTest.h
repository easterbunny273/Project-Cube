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
		TEST_ADD(GridTest::TestMirrorHorizontal);
		TEST_ADD(GridTest::TestMirrorVertical);
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Grid testgrid;

	void TestDoorInsertion();
	void TestRotation1();
	void TestRotation2();
	void TestRotation3();
	void TestMirrorHorizontal();
	void TestMirrorVertical();
};