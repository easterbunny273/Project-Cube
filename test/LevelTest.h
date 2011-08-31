#include "cpptest.h"
using namespace std;

#include "Level.h"

class LevelTest: public Test::Suite
{
public:
	LevelTest()
	{
		//Tests for normal functionality
		TEST_ADD(LevelTest::Test_loadXML_LevelAttributes)
		TEST_ADD(LevelTest::Test_writeXML)

		//Tests for error handling
		TEST_ADD(LevelTest::Test_loadXML_NotExistingLevel)
		TEST_ADD(LevelTest::Test_loadXML_WrongGroupTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongCubeTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongGridTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongDoorTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongDoorRange)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Level testlevel;
	
	//Tests for normal functionality
	void Test_loadXML_LevelAttributes();
	void Test_writeXML();

	//Tests for error handling
	void Test_loadXML_NotExistingLevel();
	void Test_loadXML_WrongGroupTag();
	void Test_loadXML_WrongCubeTag();
	void Test_loadXML_WrongGridTag();
	void Test_loadXML_WrongDoorTag();
	void Test_loadXML_WrongDoorRange();
};