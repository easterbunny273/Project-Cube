#include "Settings.h"
#include "Logger.h"


int main()
{
    // some code to demonstrate the logger class

    Logger::SetLogLevelForConsole(Logger::ERROR);
    Logger::SetLogLevelForFile(Logger::DEBUG);

    Logger::debug() << "debug message test" << Logger::endl;
    Logger::error() << "error message test" << Logger::endl;

    int a = 3;

    Logger::error() << "this class can handle different types of variables. e.g: A=" << a << Logger::endl;
}
