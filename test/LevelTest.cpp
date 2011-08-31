#include "LevelTest.h"

void LevelTest::setup()
{

}

void LevelTest::tear_down()
{
}

void LevelTest::TestLevelAttributes()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/testlevel1.xml");

	TEST_ASSERT(bRead)
	TEST_ASSERT(testlevel.GetLevelID() == 1)
	TEST_ASSERT(testlevel.GetLevelName().compare("testlevel")==0)
	TEST_ASSERT(testlevel.GetNumCubes() == 2)

}

