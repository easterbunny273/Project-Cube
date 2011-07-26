#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	 // Informiert Test-Listener ueber Testresultate
    CPPUNIT_NS :: TestResult testresult;

    // Listener zum Sammeln der Testergebnisse registrieren
    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // Listener zur Ausgabe der Ergebnisse einzelner Tests
    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // Test-Suite ueber die Registry im Test-Runner einfuegen
    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);

	filebuf fbxml;
	fbxml.open("testresults.xml", ios::out);
	ostream osxml(&fbxml);

	filebuf fbtxt;
	fbtxt.open("testresults.txt", ios::out);
	ostream ostxt(&fbtxt);

    // Resultate im xml-Format ausgeben
    CPPUNIT_NS :: XmlOutputter xmloutputter (&collectedresults, osxml, "UTF-8");
    xmloutputter.write ();

	// Resultate im txt-Format ausgeben
    CPPUNIT_NS :: TextOutputter txtoutputter (&collectedresults, ostxt);
    txtoutputter.write ();

    // Rueckmeldung, ob Tests erfolgreich waren
    return collectedresults.wasSuccessful () ? 0 : 1;
}