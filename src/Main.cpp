#include <QApplication>
#include "gui/MainWindow.h"

#include "MainApp.h"
#include "Gamelogic/Level.h"
#include "EventManager.h"

#include "Logger.h"

using namespace BambooLib;

int main(int argc, char *argv[])
{
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
