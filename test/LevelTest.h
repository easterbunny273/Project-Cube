#include "cpptest.h"
using namespace std;

#include "Level.h"

class LevelTest: public Test::Suite
{
public:
	LevelTest()
	{
		TEST_ADD(LevelTest::testLevelAttributes)
	}

protected:
	virtual void setup();
	virtual void tear_down();

private:
	Level testlevel;

	void testLevelAttributes();
};