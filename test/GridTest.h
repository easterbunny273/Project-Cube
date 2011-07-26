#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class GridTest : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE(GridTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void test1();
};