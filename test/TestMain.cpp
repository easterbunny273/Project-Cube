#include "cpptest.h"
#include <cpptest-htmloutput.h>
#include "GridTest.h"
#include "CubeTest.h"
#include <fstream>
#include <memory>

using namespace std;

int main()
{
	Test::Suite ts;
	ts.add(auto_ptr<Test::Suite> (new GridTest()));
	ts.add(auto_ptr<Test::Suite> (new CubeTest()));

	filebuf fbhtml;
	fbhtml.open("testresults.html", ios::out);
	ostream ofile(&fbhtml);

	Test::HtmlOutput output;
	ts.run(output);
	output.generate(ofile);
	return 0;
}