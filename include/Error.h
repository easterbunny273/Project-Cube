/*
 * header file for Error class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 07/2011, Project Cube
 */

#pragma once
#ifndef __ERROR_PROJECT_CUBE_HEADER
#define __ERROR_PROJECT_CUBE_HEADER

#include <stdexcept>
#include <string>

#include "Logger.h"

//*
// Should we use an own error class for error handling? How should we handle errors in subsystems ?
// At the moment, a message streamed to Logger::fatal() instantly stops the application, but I think
// in a complex application this is not the best way because maybe some cleaning up before exit() should be done?
//*

// this class is only a draft
class FatalError : public std::runtime_error
{
public:
    Error(std::string sSubsystem, std::string sErrorMessage)
        : std::runtime_error(sSubsystem + ":" + sErrorMessage)
    {

    }
};

#endif //__ERROR_PROJECT_CUBE_HEADER
