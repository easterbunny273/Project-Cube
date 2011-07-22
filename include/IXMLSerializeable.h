/*
 * header file for IXMLSerializeable interface
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 07/2011, Project "Project Cube"
 */

#pragma once

#ifndef __IXMLSERIALIZEABLE_HEADER
#define __IXMLSERIALIZEABLE_HEADER

#include <string>

class IXMLSerializeable
{
public:
    /// serializes the class as XML to the given string, returns true if success
    virtual bool WriteToXMLString(std::string &rsString) = 0;

    /// reads the class as XML from the given string, returns true if success
    virtual bool ReadFromXMLString(std::string sString) = 0;

};

#endif //__IXMLSERIALIZEABLE_HEADER
