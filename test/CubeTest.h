#include "cpptest.h"
using namespace std;

#include "Cube.h"

class CubeTest: public Test::Suite
{
public:
	CubeTest()
	{
		TEST_ADD(CubeTest::TestCubeRotationX1)
		TEST_ADD(CubeTest::TestCubeRotationX2)
		TEST_ADD(CubeTest::TestCubeRotationX3)
		TEST_ADD(CubeTest::TestCubeRotationY1)
		TEST_ADD(CubeTest::TestCubeRotationY2)
		TEST_ADD(CubeTest::TestCubeRotationY3)
		TEST_ADD(CubeTest::TestCubeRotationZ1)
		TEST_ADD(CubeTest::TestCubeRotationZ2)
		TEST_ADD(CubeTest::TestCubeRotationZ3)
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

	void TestCubeRotationX1();
	void TestCubeRotationX2();
	void TestCubeRotationX3();
	void TestCubeRotationY1();
	void TestCubeRotationY2();
	void TestCubeRotationY3();
	void TestCubeRotationZ1();
	void TestCubeRotationZ2();
	void TestCubeRotationZ3();
};