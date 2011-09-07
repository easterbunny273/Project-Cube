#include "TestMain.h"
#include <cpptest-htmloutput.h>
#include "GridTest.h"
#include "CubeTest.h"
#include "LevelTest.h"
#include <fstream>
#include <memory>

using namespace std;

TestMain::TestMain()
{
    m_TestSuite.add(auto_ptr<Test::Suite> (new GridTest()));
    m_TestSuite.add(auto_ptr<Test::Suite> (new CubeTest()));
    m_TestSuite.add(auto_ptr<Test::Suite> (new LevelTest()));
}

TestMain::~TestMain()
{
}

TestMain * TestMain::GetInstance()
{
    static TestMain instance;

    return &instance;
}

void TestMain::Run()
{
	filebuf fbhtml;
	fbhtml.open("testresults.html", ios::out);
	ostream ofile(&fbhtml);

	Test::HtmlOutput output;
        m_TestSuite.run(output);
	output.generate(ofile);
}
