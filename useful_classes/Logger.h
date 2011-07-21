/*
 * header file for Logger class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 10/2010, Project "BambooIsland"
 * 03/2011, Project "Free Pool"
 */

#pragma once

#ifndef __realtime_lu_logger
#define __realtime_lu_logger

#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include <assert.h>

#ifdef TRACE
#warning TRACE was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, TRACE was undefined!
#undef TRACE
#endif

#ifdef DEBUG
#warning DEBUG was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, DEBUG was undefined!
#undef DEBUG
#endif

#ifdef WARN
#warning WARN was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, WARN was undefined!
#undef WARN
#endif

#ifdef ERROR
#warning ERROR was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, ERROR was undefined!
#undef ERROR
#endif

#ifdef FATAL
#warning FATAL was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, FATAL was undefined!
#undef FATAL
#endif

#ifdef NOTHING
#warning NOTHING was defined as precompiler variable by a used library. To prevent incompatibilities with the class Logger, NOTHING was undefined!
#undef NOTHING
#endif

/**
 * This Logger class provides a simple interface to log messages with different importance (debug/info/warning/error/fatal)
 * and write them to the console and/or a log file.
 * The interface was designed to behave like the standard c++ output interface, "cout", and is implemented as a singelton class.
 *
 * Usage examples:
 *   first set loglevels:   Logger::setLogLevelForConsole(Logger::ERROR);
 *			    Logger::setLogLevelForFile(Logger::DEBUG);
 *
 *  then write to different targets just like to cout:
 *	Logger::error() << "This is an error message" << Logger::endl;
 *	Logger::debug() << "This is a debug message" << Logger::endl;
 *
 *
 */
class Logger
{
public:
    enum LogLevel
    {
	DEBUG=0,
	INFO,
	ERROR,
	FATAL,
	NOTHING ///< if no output is wanted, set the loglevel to NOTHING
    };

    class TItlNestedEndl
    {
    };

    static TItlNestedEndl endl;

    class TItlLogTarget
    {
    private:
	LogLevel m_level;

    public:
	TItlLogTarget(LogLevel level) : m_level(level) {};

	TItlLogTarget& operator<<(TItlNestedEndl p)
	{
	    if (Logger::instance()->loglevel_console <= m_level)
	    {
		if (m_level >= ERROR)
		    std::cerr << std::endl;
		else
		    std::cout << std::endl;

	    }

	    if (Logger::instance()->loglevel_file <= m_level)
		Logger::instance()->logfile << std::endl;

	    if (m_level >= FATAL)
	    {
		assert (!"fatal error");
		throw new std::runtime_error("");
	    }


	    return *this;
	}

	template <typename T> TItlLogTarget& operator<<(T a)
	{
	    if (Logger::instance()->loglevel_console <= m_level)
	    {
		if (m_level >= ERROR)
		    std::cerr << a;
		else
		    std::cout << a;
	    }

	    if (Logger::instance()->loglevel_file <= m_level)
		Logger::instance()->logfile << a;


	    return *this;
	}
    };


    static TItlLogTarget DebugTarget;
    static TItlLogTarget InfoTarget;
    static TItlLogTarget ErrorTarget;
    static TItlLogTarget FatalTarget;

    static TItlLogTarget &debug()
    {
	if (Logger::instance()->loglevel_console <= DEBUG)
	{
	    std::cout << "DEBUG: ";
	}

	if (Logger::instance()->loglevel_file <= DEBUG)
	{
	    Logger::instance()->logfile << "DEBUG: ";
	}

	return DebugTarget;
    }

    static TItlLogTarget &info()
    {
	if (Logger::instance()->loglevel_console <= INFO)
	{
	    std::cout << "INFO: ";
	}

	if (Logger::instance()->loglevel_file <= INFO)
	{
	    Logger::instance()->logfile << "INFO: ";
	}

	return InfoTarget;
    }

    static TItlLogTarget &error()
    {
	if (Logger::instance()->loglevel_console <= ERROR)
	{
	    std::cerr << "ERROR: ";
	}

	if (Logger::instance()->loglevel_file <= ERROR)
	{
	    Logger::instance()->logfile << "ERROR: ";
	}

	return ErrorTarget;
    }

    static TItlLogTarget &fatal()
    {
	if (Logger::instance()->loglevel_console <= FATAL)
	{
	    std::cerr << "FATAL: ";
	}

	if (Logger::instance()->loglevel_file <= FATAL)
	{
	    Logger::instance()->logfile << "FATAL: ";
	}

	return FatalTarget;
    }




    /// function to set log level for file (messages with a lower level are ignored)
    static void setLogLevelForFile(LogLevel loglevel);

    /// function to set log level for console (messages with a lower level are ignored)
    static void setLogLevelForConsole(LogLevel loglevel);




private:
    ///Constructor (private because singelton class)
    Logger();

    ~Logger();

    /// returns instance of logger class (-> logger is designed as a singelton)
    static Logger* instance();

    LogLevel loglevel_file;
    LogLevel loglevel_console;

    std::fstream logfile;

    /// intern function to transform a loglevel in a describing char*
    const char * getLevelName(LogLevel level);
};

#endif
