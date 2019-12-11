#include "FileLogger.h"

#include <cassert>
#include <ctime>
#include <iomanip>

namespace
{
    const char* HeaderForSeverity(LogSeverity severity)
    {
        switch (severity)
        {
        case LogSeverity::Information:
            return "INFO";
        case LogSeverity::Warning:
            return "WARN";
        case LogSeverity::Critical:
            return "CRIT";
        default:
            return "UNKN";
            assert(false);
        }
    }

    void WriteTimeToStream(std::ostream& fileStream)
    {
        // Get current time.
        auto currentTime = std::time(nullptr);

        // Convert to current time zone.
        auto currentLocalTime = std::localtime(&currentTime);

        // Write to stream.
        fileStream << std::put_time(currentLocalTime, "%d-%m-%Y %H:%M:%S");
    }

    void WriteLogEntry(
        std::ofstream& fileStream,
        LogSeverity severity,
        const std::string& file,
        int line,
        const std::string& message)
    {
        // Write severity header.
        fileStream << HeaderForSeverity(severity);
        fileStream << ':' << ' ';

        // Write the current time.
        WriteTimeToStream(fileStream);

        // Write the component name.
        fileStream << ' ' << '-' << ' ';
        fileStream << file << ':' << line;

        // Write the message.
        fileStream << ' ' << '-' << ' ';
        fileStream << message;

        // Write terminating end line.
        fileStream << '\r' << '\n';
    }
}

FileLogger::FileLogger(const std::string& filePath, LogSeverity severity) : filePath(filePath), fileStream(nullptr)
{
    this->GetReportingSeverity().SetValue(severity);
}

FileLogger::~FileLogger()
{
    // Lock file stream.
    this->fileStream.Access([](std::ofstream*& fileStream)
    {
        // Close file.
        if (fileStream != nullptr)
        {
            fileStream->close();
            fileStream = nullptr;
        }
    });
}

void FileLogger::__Log(LogSeverity severity, const std::string& file, int line, const std::string& message)
{
    // Bail if log level is too low.
    LogSeverity reportingSeverity = this->GetReportingSeverity().GetValue();
    if (severity < reportingSeverity)
    {
        return;
    }

    auto filePath = this->filePath;

    // Lock file stream object.
    this->fileStream.Access([filePath, severity, file, line, message](std::ofstream*& fileStream)
    {
        // Lazily create the file stream if needed.
        if (fileStream == nullptr)
        {
            fileStream = new std::ofstream(filePath, std::ios::out);
        }

        // Write the entry.
        WriteLogEntry(*fileStream, severity, file, line, message);
    });
}

void FileLogger::__LogAndFailFast(
    const std::string& file,
    int line,
    const std::string& message,
    ExitCode exitCode)
{
    // Write to the log.
    this->Log(LogSeverity::Critical, file, line, message);

    // Lock file stream.
    this->fileStream.Access([exitCode](std::ofstream*& fileStream)
    {
        // Close file to ensure it's all written.
        if (fileStream != nullptr)
        {
            fileStream->close();
            fileStream = nullptr;
        }

        // Bail ASAP inside the mutex to ensure no writes happen after close.
        exit(static_cast<int>(exitCode));
    });
}
