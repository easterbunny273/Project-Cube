/*
 * header file for BambooLib::Logger class
 * written by: christian moellinger <ch.moellinger@gmail.com>
 * 05/2012, BambooLib
 */

#ifndef BAMBOOLIB_LOGGER_H
#define BAMBOOLIB_LOGGER_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace BambooLib
{
class Logger
{
public:
    /*! \name Public types */
    //@{
    /// a nested class, only needed to define a special type, thus the class is empty
    class TItlNestedEndl
    {
    };

    /// the special type to define a end-of-line
    static TItlNestedEndl endl;

    /// the enum which defines the level of the log event
    enum TLogLevel
    {
        DEBUG=0,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        NOTHING ///< if no output is wanted, set the loglevel to NOTHING
    };

    /// the interface for logwriter classes
    class ILogWriter
    {
        friend class Logger;
    public:
        /// Set the log level under which the messages should be ignored.
        void SetIgnoreBelow(TLogLevel eIgnoreBelow) { m_eIgnoreBelowLogLevel = eIgnoreBelow; }

        /// is called by the logger class, the concrete ILogWriter class has to handle the message
        virtual void Write(TLogLevel eLevel, long int lUnixTimestamp, const char *szMessage) = 0;

        /// destructor
        virtual ~ILogWriter() {}

    protected:
        Logger::TLogLevel m_eIgnoreBelowLogLevel;
        ILogWriter() : m_eIgnoreBelowLogLevel(Logger::DEBUG) {}
    };

    /// a concrete implementation of ILogWriter, which outputs each log message on cout or cerr (depending on level)
    class ConsoleLogWriter : public ILogWriter
    {
    public:
        static ConsoleLogWriter * Create();
        virtual void Write(TLogLevel eLevel, long int lUnixTimestamp, const char *szMessage);
    };

    /// a concrete implementation of ILogWriter, which writes each message in the given file
    class FileLogWriter : public ILogWriter
    {
    public:
        static FileLogWriter * Create(const std::string &sFilename);
        virtual void Write(TLogLevel eLevel, long int lUnixTimestamp, const char *szMessage);

        virtual ~FileLogWriter();

    private:
        FileLogWriter();

        std::string m_sFilename;
        std::ofstream * m_pFile;
    };

    /// a concrete implementation of ILogWriter, which writes each message in the given html-file
    class HTMLLogWriter : public ILogWriter
    {
    public:
        static HTMLLogWriter * Create(const std::string &sFilename);
        virtual void Write(TLogLevel eLevel, long int lUnixTimestamp, const char *szMessage);

        virtual ~HTMLLogWriter();
    private:
        HTMLLogWriter();
        std::string m_sFilename;
        std::ofstream * m_pFile;
    };

    /// the nested class for the log targets
    class TItlLogTarget
    {
    private:
        TLogLevel m_eLevel;
        std::stringstream sBuffer;

    public:
        TItlLogTarget(TLogLevel eLevel) : m_eLevel(eLevel) {}

        /// the operator << which accepts only Logger::endl
        TItlLogTarget& operator<<(TItlNestedEndl p);

        /// the operator << which accepts all other types which are accepted by the standard c++-streams
        /// the compiler will generate an own method for each type which is used as a parameter
        template <typename T> TItlLogTarget& operator<<(T a)
        {
            sBuffer << a;
            return *this;
        }
    };

    //@}

    /*! \name Construction / Destruction */
    //@{

    /// constructor
    Logger();

    /// destructor
    ~Logger();

    //@}

    /*! \name Static methods */
    //@{

    /// logs a message
    static void LogMessage(TLogLevel eLevel, const std::string &rsMessage) { LogMessage(eLevel, rsMessage.c_str()); }

    /// logs a message
    static void LogMessage(TLogLevel eLevel, const char * szMessage);

    /// returns the buffer targets for each level
    static TItlLogTarget &debug()     {         return m_rBufferTarget_Debug;      }
    static TItlLogTarget &info()      {         return m_rBufferTarget_Info;       }
    static TItlLogTarget &warning()   {         return m_rBufferTarget_Warning;    }
    static TItlLogTarget &error()     {         return m_rBufferTarget_Error;      }
    static TItlLogTarget &fatal()     {         return m_rBufferTarget_Fatal;      }

    /// registers a new log writer to the Logger
    /// the writer will be deleted by the logger at the end of the program execution time
    static bool RegisterLogWriter(ILogWriter *pWriter);

    /// unregisters a given log writer and deletes it
    static bool UnregisterAndDeleteLogWriter(ILogWriter *pWriter);

    /// returns a string which describes the given level
    static const char * GetLevelString(TLogLevel eLevel);
    //@}

private:
    /*! \name Private members */
    //@{
    static TItlLogTarget             m_rBufferTarget_Debug;
    static TItlLogTarget             m_rBufferTarget_Info;
    static TItlLogTarget             m_rBufferTarget_Warning;
    static TItlLogTarget             m_rBufferTarget_Error;
    static TItlLogTarget             m_rBufferTarget_Fatal;

    static std::vector<ILogWriter *> m_vLogWriters;
    //@}
};
}
#endif // LOGGER_H
