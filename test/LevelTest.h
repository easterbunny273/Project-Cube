#include "cpptest.h"
using namespace std;

#include "Level.h"

class LevelTest: public Test::Suite
{
public:
	LevelTest()
	{
		TEST_ADD(LevelTest::TestLevelAttributes)
		TEST_ADD(LevelTest::TestWrongGroupTag)
		TEST_ADD(LevelTest::TestWrongCubeTag)
		TEST_ADD(LevelTest::TestWrongGridTag)
		TEST_ADD(LevelTest::TestWrongDoorTag)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Level testlevel;

	void TestLevelAttributes();
	void TestWrongGroupTag();
	void TestWrongCubeTag();
	void TestWrongGridTag();
	void TestWrongDoorTag();
};