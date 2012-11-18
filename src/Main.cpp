#include <QApplication>
#include "gui/MainWindow.h"

#include "MainApp.h"
#include "Gamelogic/Level.h"
#include "EventManager.h"

#include "Logger.h"
#include "KeyValueMap.h"
#include "ConstrainedKeyValueMap.h"

using namespace BambooLib;

int main(int argc, char *argv[])
{
    KeyValueMap test;
    test.SetValue("test-float", 3.0f);

    KeyValueMap test2 = test;


    std::map<KeyValueMap::TKey, ConstrainedKeyValueMap::TConstraint> mConstrainsMap;
    std::vector<std::string> vsTestPossibleValues;
    vsTestPossibleValues.push_back("style1");
    vsTestPossibleValues.push_back("style2");
    vsTestPossibleValues.push_back("other-style");

    mConstrainsMap["render-style"] = ConstrainedKeyValueMap::CreateConstraint(vsTestPossibleValues);
    mConstrainsMap["a-float-key"] = ConstrainedKeyValueMap::CreateConstraint(KeyValueMap::VALUE_FLOAT);
    mConstrainsMap["a-ranged-double-key"] = ConstrainedKeyValueMap::CreateConstraint(KeyValueMap::VALUE_DOUBLE, 2.0, 10.0, 0.5);

    ConstrainedKeyValueMap Test3(mConstrainsMap, true);

    bool bTest1 = Test3.SetValue("render-style", std::string("style1"));
    bool bTest2 = Test3.SetValue("render-style", std::string("stAle1"));
    bool bTest3 = Test3.SetValue("render-style", std::string("style2"));
    bool bTest4 = Test3.SetValue("a-float-key", 3.0);
    bool bTest5 = Test3.SetValue("a-float-key", 3.0f);
    bool bTest6 = Test3.SetValue("a-ranged-double-key", 1.0);
    bool bTest7 = Test3.SetValue("a-ranged-double-key", 10.1);
    bool bTest8 = Test3.SetValue("a-ranged-double-key", 7.1);
    bool bTest9 = Test3.SetValue("a-ranged-double-key2", 7.1);

    const ConstrainedKeyValueMap::TConstraint * pTestContraint = Test3.GetConstraint("a-ranged-double-key");

    float fTest;
    bool bOk = test2.GetValue("test-float", fTest);

    QApplication a(argc, argv);
    MainWindow MainWindow;

    Logger::ILogWriter *pLogWriterConsole = Logger::ConsoleLogWriter::Create();
    Logger::ILogWriter *pLogWriterHTML = Logger::HTMLLogWriter::Create("log.html");

    pLogWriterConsole->SetIgnoreBelow(Logger::DEBUG);
    pLogWriterHTML->SetIgnoreBelow(Logger::DEBUG);

    Logger::RegisterLogWriter(pLogWriterConsole);
    Logger::RegisterLogWriter(pLogWriterHTML);

    MainWindow.show();

    MainApp::GetInstance()->Run(&a, MainWindow.GetGLWidget());

    int iReturnValue = a.exec();

    delete pLogWriterConsole;
    delete pLogWriterHTML;

    return iReturnValue;
}
