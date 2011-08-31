#include "LevelTest.h"

void LevelTest::setup()
{

}

void LevelTest::tear_down()
{
}

void LevelTest::Test_loadXML_LevelAttributes()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/testlevel1.xml");

	TEST_ASSERT(bRead)
	TEST_ASSERT(testlevel.GetLevelID() == 1)
	TEST_ASSERT(testlevel.GetLevelName().compare("testlevel")==0)
	TEST_ASSERT(testlevel.GetNumCubes() == 2)
}

void LevelTest::Test_loadXML_NotExistingLevel()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/abasobaba.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongGroupTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongGroupTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongCubeTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongCubeTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongGridTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongGridTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongDoorTag()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongDoorTag.xml");
	TEST_ASSERT(bRead==false)
}

void LevelTest::Test_loadXML_WrongDoorRange()
{
	bool bRead = testlevel.ReadFromXMLString("test/config/levels/LevelWrongDoorRange.xml");
	TEST_ASSERT(bRead==false)
}