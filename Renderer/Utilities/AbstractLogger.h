#pragma once

#include "../Utilities/Property.h"
#include <string>

#define Log(severity, message) \
    __Log(severity, std::string(__FILE__), __LINE__, message)

#define LogAndFailFast(message) \
    __LogAndFailFast(std::string(__FILE__), __LINE__, message)

enum class ExitCode
{
    Success = 0,
    Failure = 1
};

enum class LogSeverity
{
    Information = 0,
    Warning = 1,
    Critical = 2
};

class AbstractLogger
{
public:
    virtual void __Log(LogSeverity severity, const std::string& file, int line, const std::string& message) = 0;
    virtual void __LogAndFailFast(
        const std::string& file,
        int line,
        const std::string& message,
        ExitCode exitCode = ExitCode::Failure) = 0;

    Property<LogSeverity>& GetReportingSeverity() { return this->reportingSeverity; }

private:
    Property<LogSeverity> reportingSeverity = LogSeverity::Critical;
};
