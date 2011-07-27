#include "cpptest.h"
#include <cpptest-htmloutput.h>
#include "GridTest.h"
#include <fstream>

using namespace std;

int main()
{
	GridTest gridtest;
	filebuf fbhtml;
	fbhtml.open("testresults.html", ios::out);
	ostream ofile(&fbhtml);
	Test::HtmlOutput output;
	gridtest.run(output);
	output.generate(ofile);
	return 0;
}