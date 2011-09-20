#pragma once
#ifndef __CUBETEST_PROJECTCUBE_TEST_HEADER
#define __CUBETEST_PROJECTCUBE_TEST_HEADER


#include "cpptest.h"
using namespace std;

#include "Gamelogic/Cube.h"

class CubeTest: public Test::Suite
{
public:
    /*! \name Construction / Destruction */
    //@{
        /// Constructor
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
    //@}

protected:
    /*! \name Setup and tear_down */
    //@{
        /// Setup before every test
	virtual void setup();
        /// Tear down after every test
	virtual void tear_down();
    //@}

private:
    /*! \name Test members */
    //@{
	Grid grid1;
	Grid grid2;
	Grid grid3;
	Grid grid4;
	Grid grid5;
	Grid grid6;

	Cube testcube;
    //@}

    /*! \name Tests */
    //@{
	void TestCubeRotationX90();
	void TestCubeRotationX180();
	void TestCubeRotationX270();
	void TestCubeRotationY90();
	void TestCubeRotationY180();
	void TestCubeRotationY270();
	void TestCubeRotationZ90();
	void TestCubeRotationZ180();
	void TestCubeRotationZ270();
    //@}
};

#endif //__CUBETEST_PROJECTCUBE_TEST_HEADER
