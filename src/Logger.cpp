/*
 * source file for Logger class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Logger.h"

using namespace std;

Logger::TItlLogTarget Logger::DebugTarget(Logger::DEBUG);
Logger::TItlLogTarget Logger::InfoTarget(Logger::INFO);
Logger::TItlLogTarget Logger::ErrorTarget(Logger::ERROR);
Logger::TItlLogTarget Logger::FatalTarget(Logger::FATAL);

Logger::TItlNestedEndl Logger::endl;

Logger *Logger::instance()
{
    static Logger singelton_instance;
    return &singelton_instance;
}

Logger::Logger()
{
    this->logfile.open("log.txt", ios::out);
}

Logger::~Logger()
{
    this->logfile.close();
}

void Logger::SetLogLevelForConsole(LogLevel loglevel)
{
    instance()->loglevel_console = loglevel;
}

void Logger::SetLogLevelForFile(LogLevel loglevel)
{
    instance()->loglevel_file = loglevel;
}

const char *Logger::getLevelName(LogLevel level)
{
    switch (level)
    {
    case DEBUG:
	return "DEBUG";
	break;
    case INFO:
	return "WARNING";
	break;
    case ERROR:
	return "ERROR";
	break;
    case FATAL:
	return "FATAL";
	break;
    default:
	throw new std::invalid_argument("wrong loglevel used!");
    }
}
