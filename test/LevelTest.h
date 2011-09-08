#include "cpptest.h"
using namespace std;

#include "Gamelogic/Level.h"

class LevelTest: public Test::Suite
{
public:
	LevelTest()
	{
		//Tests for normal functionality
		TEST_ADD(LevelTest::Test_loadXML_LevelAttributes)
                TEST_ADD(LevelTest::Test_loadXML_Cubes)
		TEST_ADD(LevelTest::Test_writeXML)
                TEST_ADD(LevelTest::Test_RotateLevelX90)
                TEST_ADD(LevelTest::Test_RotateLevelX180)
                TEST_ADD(LevelTest::Test_RotateLevelX270)
                TEST_ADD(LevelTest::Test_RotateLevelY90)
                TEST_ADD(LevelTest::Test_RotateLevelY180)
                TEST_ADD(LevelTest::Test_RotateLevelY270)
                TEST_ADD(LevelTest::Test_RotateLevelZ90)
                TEST_ADD(LevelTest::Test_RotateLevelZ180)
                TEST_ADD(LevelTest::Test_RotateLevelZ270)

		//Tests for error handling
		TEST_ADD(LevelTest::Test_loadXML_NotExistingLevel)
		TEST_ADD(LevelTest::Test_loadXML_WrongGroupTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongCubeTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongGridTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongDoorTag)
		TEST_ADD(LevelTest::Test_loadXML_WrongDoorRange)
                TEST_ADD(LevelTest::Test_NotExistingCubeInLevel)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Level testlevel;
	
	//Tests for normal functionality
	void Test_loadXML_LevelAttributes();
        void Test_loadXML_Cubes();
	void Test_writeXML();
        void Test_RotateLevelX90();
        void Test_RotateLevelX180();
        void Test_RotateLevelX270();
        void Test_RotateLevelY90();
        void Test_RotateLevelY180();
        void Test_RotateLevelY270();
        void Test_RotateLevelZ90();
        void Test_RotateLevelZ180();
        void Test_RotateLevelZ270();

	//Tests for error handling
	void Test_loadXML_NotExistingLevel();
	void Test_loadXML_WrongGroupTag();
	void Test_loadXML_WrongCubeTag();
	void Test_loadXML_WrongGridTag();
	void Test_loadXML_WrongDoorTag();
	void Test_loadXML_WrongDoorRange();
        void Test_NotExistingCubeInLevel();
};
