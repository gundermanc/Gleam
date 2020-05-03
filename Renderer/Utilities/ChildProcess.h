#pragma once

#include <string>

#include "AbstractLogger.h"

class ChildProcess {
public:
    ChildProcess(AbstractLogger& logger, const std::string& path);
    ~ChildProcess();
    void Start();
    void Stop();
    bool GetIsRunning();
    const std::string ReadStdOutLine();
    void WriteStdInLine(const std::string& line);

private:
    const std::string path;
    AbstractLogger& logger;

    // Uses naked pointers here to avoid polluting calling code with references
    // to platform specific types.
    void* processInfo;
    void* stdInRead;
    void* stdInWrite;
    void* stdOutRead;
    void* stdOutWrite;
};