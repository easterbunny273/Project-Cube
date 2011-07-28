#include "cpptest.h"
using namespace std;

#include "Cube.h"

class CubeTest: public Test::Suite
{
public:
	CubeTest()
	{
		TEST_ADD(CubeTest::TestCubeRotationX90)
		TEST_ADD(CubeTest::TestCubeRotationX180)
		TEST_ADD(CubeTest::TestCubeRotationX270)
		TEST_ADD(CubeTest::TestCubeRotationY90)
		TEST_ADD(CubeTest::TestCubeRotationY180)
		TEST_ADD(CubeTest::TestCubeRotationY270)
		TEST_ADD(CubeTest::TestCubeRotationZ90)
		TEST_ADD(CubeTest::TestCubeRotationZ180)
		TEST_ADD(CubeTest::TestCubeRotationZ270)
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

	void TestCubeRotationX90();
	void TestCubeRotationX180();
	void TestCubeRotationX270();
	void TestCubeRotationY90();
	void TestCubeRotationY180();
	void TestCubeRotationY270();
	void TestCubeRotationZ90();
	void TestCubeRotationZ180();
	void TestCubeRotationZ270();
};