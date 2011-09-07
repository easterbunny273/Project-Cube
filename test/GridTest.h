#include "cpptest.h"
using namespace std;

#include "Gamelogic/Grid.h"

class GridTest : public Test::Suite {

public:
	GridTest()
	{
		TEST_ADD(GridTest::TestDoorInsertion)
		TEST_ADD(GridTest::TestRotation90)
		TEST_ADD(GridTest::TestRotation180)
		TEST_ADD(GridTest::TestRotation270)
		TEST_ADD(GridTest::TestMirrorHorizontal)
		TEST_ADD(GridTest::TestMirrorVertical)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Grid testgrid;

	void TestDoorInsertion();
	void TestRotation90();
	void TestRotation180();
	void TestRotation270();
	void TestMirrorHorizontal();
	void TestMirrorVertical();
};
