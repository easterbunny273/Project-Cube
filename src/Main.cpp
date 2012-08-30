#include <QCoreApplication>


#include "MainApp.h"
#include "Gamelogic/Level.h"


#include "EventManager.h"

#include "Logger.h"

using namespace BambooLib;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Logger::ILogWriter *pLogWriterConsole = Logger::ConsoleLogWriter::Create();
    Logger::ILogWriter *pLogWriterHTML = Logger::HTMLLogWriter::Create("log.html");

    pLogWriterConsole->SetIgnoreBelow(Logger::DEBUG);
    pLogWriterHTML->SetIgnoreBelow(Logger::DEBUG);

    Logger::RegisterLogWriter(pLogWriterConsole);
    Logger::RegisterLogWriter(pLogWriterHTML);

    MainApp::GetInstance()->Run();

    int iReturnValue = a.exec();

    delete pLogWriterConsole;
    delete pLogWriterHTML;

    return iReturnValue;
}
