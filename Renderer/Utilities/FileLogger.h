#pragma once

#include "AbstractLogger.h"
#include "Synchronized.h"
#include <fstream>
#include <string>

class FileLogger : public AbstractLogger
{
public:
    // TODO: waht do on failure?
    FileLogger(const std::string& filePath, LogSeverity severity = LogSeverity::Critical);
    ~FileLogger();

    // Overrides
    void __Log(LogSeverity severity, const std::string& file, int line, const std::string& message);
    void __LogAndFailFast(
        const std::string& file,
        int line,
        const std::string& message,
        ExitCode exitCode = ExitCode::Failure);

private:
    const std::string filePath;
    Synchronized<std::ofstream*> fileStream;
};
