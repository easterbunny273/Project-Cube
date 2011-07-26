#include "GridTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GridTest);

void GridTest::setUp()
{
}

void GridTest::tearDown()
{
}

void GridTest::test1()
{
	CPPUNIT_ASSERT_EQUAL(1 + 1, 2);
}