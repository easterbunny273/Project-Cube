#include "cpptest.h"
using namespace std;

class GridTest : public Test::Suite {

public:
	GridTest()
	{
		TEST_ADD(GridTest::test1);
	}

private:
	void test1();
};