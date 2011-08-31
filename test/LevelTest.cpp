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

void LevelTest::TestWrongGroupTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongGroupTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::TestWrongCubeTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongCubeTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::TestWrongGridTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongGridTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::TestWrongDoorTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongDoorTag.xml");
	TEST_ASSERT(bRead==false)
}