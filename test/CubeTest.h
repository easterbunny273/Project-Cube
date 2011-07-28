#include "cpptest.h"
using namespace std;

#include "Cube.h"

class CubeTest: public Test::Suite
{
public:
	CubeTest()
	{
		TEST_ADD(CubeTest::TestCubeRotation)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Grid grid1;
	Grid grid2;
	Grid grid3;
	Grid grid4;
	Grid grid5;
	Grid grid6;

	Cube testcube;

	void TestCubeRotation();
};