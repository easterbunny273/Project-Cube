#pragma once
#ifndef __GRIDTEST_PROJECTCUBE_TEST_HEADER
#define __GRIDTEST_PROJECTCUBE_TEST_HEADER



#include "cpptest.h"
using namespace std;

#include "Gamelogic/Grid.h"

class GridTest : public Test::Suite {

public:
    /*! \name Construction / Destruction */
    //@{
        /// Constructor
	GridTest()
	{
		TEST_ADD(GridTest::TestDoorInsertion)
		TEST_ADD(GridTest::TestRotation90)
		TEST_ADD(GridTest::TestRotation180)
		TEST_ADD(GridTest::TestRotation270)
		TEST_ADD(GridTest::TestMirrorHorizontal)
		TEST_ADD(GridTest::TestMirrorVertical)
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
        Grid testgrid;
    //@}

    /*! \name Tests */
    //@{
	void TestDoorInsertion();
	void TestRotation90();
	void TestRotation180();
	void TestRotation270();
	void TestMirrorHorizontal();
	void TestMirrorVertical();
    //@}
};

#endif // __GRIDTEST_PROJECTCUBE_TEST_HEADER
