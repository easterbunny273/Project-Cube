#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <cassert>
#include <ctime>

#include "BambooLib/include/Logger.h"


namespace BambooLib
{

std::vector<Logger::ILogWriter *> Logger::m_vLogWriters;

Logger::TItlLogTarget             Logger::m_rBufferTarget_Debug(Logger::DEBUG);
Logger::TItlLogTarget             Logger::m_rBufferTarget_Info(Logger::INFO);
Logger::TItlLogTarget             Logger::m_rBufferTarget_Warning(Logger::WARNING);
Logger::TItlLogTarget             Logger::m_rBufferTarget_Error(Logger::ERROR);
Logger::TItlLogTarget             Logger::m_rBufferTarget_Fatal(Logger::FATAL);

bool Logger::RegisterLogWriter(Logger::ILogWriter *pWriter)
{
    // assertion that the new pWriter is not in the list already
    //std::for_each(m_vLogWriters.begin(), m_vLogWriters.end(), [](Logger::ILogWriter *pVectorEntry){ assert(pVectorEntry != pWriter); });
    for (auto iter=m_vLogWriters.begin(); iter != m_vLogWriters.end(); iter++)
        assert(*iter != pWriter);

    m_vLogWriters.push_back(pWriter);

    return true;
}

bool Logger::UnregisterAndDeleteLogWriter(Logger::ILogWriter *pWriter)
{
    bool bFound = false;

    for (auto iter=m_vLogWriters.begin(); iter != m_vLogWriters.end(); iter++)
    {
        if (*iter == pWriter)
        {
            bFound = true;

            // remove pWriter from vector
            m_vLogWriters.erase(iter);

            // delete log writer
            delete *iter;
        }
    }

    assert (bFound);

    return bFound;
}

const char *Logger::GetLevelString(Logger::TLogLevel eLevel)
{
    switch(eLevel)
    {
    case DEBUG: return "DEBUG";
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
    case FATAL: return "FATAL";
    default: assert(!"should not come here");
    }
}

Logger::Logger()
{
    // nothing to do
}

Logger::~Logger()
{
    for (auto iter=m_vLogWriters.begin(); iter != m_vLogWriters.end(); iter++)
    {
        // delete log writer
        delete *iter;
    }

    m_vLogWriters.clear();
}

void Logger::LogMessage(Logger::TLogLevel eLevel, const char *szMessage)
{
    long int iTimestamp = time(NULL);

    for (auto iter=m_vLogWriters.begin(); iter != m_vLogWriters.end(); iter++)
    {
        (*iter)->Write(eLevel, iTimestamp, szMessage);
    }
}

Logger::ConsoleLogWriter * Logger::ConsoleLogWriter::Create()
{
    ConsoleLogWriter *pNewLogWriter = new ConsoleLogWriter;

    return pNewLogWriter;
}

void Logger::ConsoleLogWriter::Write(Logger::TLogLevel eLevel,
                                     long lUnixTimestamp,
                                     const char *szMessage)
{
    if (eLevel >= m_eIgnoreBelowLogLevel)
    {
        struct tm * pTimeinfo = localtime ( &lUnixTimestamp );

        char szBuffer[80];
        strftime (szBuffer,80,"%H:%M:%S", pTimeinfo);

        if (eLevel >= Logger::ERROR)
            std::cerr << "(" << szBuffer << ", " << Logger::GetLevelString(eLevel) << ") " << szMessage << std::endl << std::flush;
        else
            std::cout << "(" << szBuffer << ", " << Logger::GetLevelString(eLevel) << ") " << szMessage << std::endl << std::flush;
    }
}

Logger::FileLogWriter * Logger::FileLogWriter::Create(const std::string &sFilename)
{
    Logger::FileLogWriter *pNewLogWriter = new FileLogWriter();

    pNewLogWriter->m_sFilename = sFilename;
    pNewLogWriter->m_pFile = new std::ofstream(sFilename);

    return pNewLogWriter;
}

void Logger::FileLogWriter::Write(Logger::TLogLevel eLevel, long lUnixTimestamp, const char *szMessage)
{
    assert (m_pFile != NULL);
    assert (m_pFile->is_open());

    if (eLevel >= m_eIgnoreBelowLogLevel)
    {
        struct tm * pTimeinfo = localtime ( &lUnixTimestamp );

        char szBuffer[80];
        strftime (szBuffer,80,"%H:%M:%S", pTimeinfo);

        (*m_pFile) << "(" << szBuffer << ", " << Logger::GetLevelString(eLevel) << ") " << szMessage << std::endl;
    }
}

Logger::FileLogWriter::FileLogWriter()
{
    m_pFile = NULL;
}

Logger::FileLogWriter::~FileLogWriter()
{
    assert (m_pFile != NULL);
    assert (m_pFile->is_open());

    m_pFile->close();

    delete m_pFile;
    m_pFile = NULL;
}

Logger::HTMLLogWriter *Logger::HTMLLogWriter::Create(const std::string &sFilename)
{
    Logger::HTMLLogWriter *pNewLogWriter = new HTMLLogWriter();

    pNewLogWriter->m_sFilename = sFilename;
    pNewLogWriter->m_pFile = new std::ofstream(sFilename);

    // make sure that file could be opened
    // todo: proper error handling (what to do when file could not opened?)
    assert (pNewLogWriter->m_pFile->is_open());

    // print html header
    *(pNewLogWriter->m_pFile) << "<!DOCTYPE HTML>" << std::endl;
    *(pNewLogWriter->m_pFile) << "<html>" << std::endl;
    *(pNewLogWriter->m_pFile) << "<head>" << std::endl;
    *(pNewLogWriter->m_pFile) << "<style type=\"text/css\">" << std::endl;
    *(pNewLogWriter->m_pFile) << "body { font-size: 10pt; font-family: monospace; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span#time { font-weight: bold; display: inline-block; padding-right: 10px; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span#loglevel { display: inline-block; padding-right: 10px; text-align: center; width: 80px; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span.DEBUG { color: black; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span.INFO { color: green; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span.WARNING { color: orange; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span.ERROR { color: red; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "span.FATAL { color: red; font-weight: bold; background-color: yellow; }" << std::endl;
    *(pNewLogWriter->m_pFile) << "</style>" << std::endl;
    *(pNewLogWriter->m_pFile) << "</head>" << std::endl;
    *(pNewLogWriter->m_pFile) << "\t<body>" << std::endl;

    return pNewLogWriter;
}

void Logger::HTMLLogWriter::Write(Logger::TLogLevel eLevel, long lUnixTimestamp, const char *szMessage)
{
    assert (m_pFile != NULL);
    assert (m_pFile->is_open());

    if (eLevel >= m_eIgnoreBelowLogLevel)
    {
        struct tm * pTimeinfo = localtime ( &lUnixTimestamp );

        char szBuffer[80];
        strftime (szBuffer,80,"%H:%M:%S", pTimeinfo);

        const char *szLevel = Logger::GetLevelString(eLevel);

        (*m_pFile) << "\t\t<span id=\"time\">" << szBuffer << "</span><span id=\"loglevel\" class=\"" << szLevel << "\">" << szLevel << "</span>"
                   << "<span id=\"message\" class=\"" << szLevel << "\">" << szMessage << "</span><br/>" << std::endl;
    }
}

Logger::HTMLLogWriter::~HTMLLogWriter()
{
    assert (m_pFile != NULL);
    assert (m_pFile->is_open());

    (*m_pFile) << "\t</body>" << std::endl;
    (*m_pFile) << "</html>" << std::endl;

    m_pFile->close();

    delete m_pFile;
    m_pFile = NULL;
}

Logger::HTMLLogWriter::HTMLLogWriter()
{
    m_pFile = NULL;
}

Logger::TItlLogTarget &Logger::TItlLogTarget::operator <<(Logger::TItlNestedEndl p)
{
    // create string of stringstream
    std::string sMessage = sBuffer.str();

    // log message
    Logger::LogMessage(m_eLevel, sMessage);

    // clear stringstream
    sBuffer.str(std::string());
    sBuffer.clear();

    return *this;
}

}
