#include "ChildProcess.h"

#include <Windows.h>

// TODO: implement for Mac.

ChildProcess::ChildProcess(AbstractLogger& logger, const std::string& path)
    : logger(logger), path(path)
{
    this->logger.Log(LogSeverity::Information, "Creating child process object " + path);

    // Initialize handles.
    this->stdInRead = INVALID_HANDLE_VALUE;
    this->stdInWrite = INVALID_HANDLE_VALUE;
    this->stdOutRead = INVALID_HANDLE_VALUE;
    this->stdOutWrite = INVALID_HANDLE_VALUE;

    this->processInfo = new PROCESS_INFORMATION;
    ZeroMemory(this->processInfo, sizeof(PROCESS_INFORMATION));

    this->logger.Log(LogSeverity::Information, "Done creating child process object " + path);
}

ChildProcess::~ChildProcess()
{
    this->logger.Log(LogSeverity::Information, "Cleaning up child process " + this->path);

    this->Stop();

    this->logger.Log(LogSeverity::Information, "Closing child process handles.");

    if (!CloseHandle(this->stdInRead))
    {
        this->logger.Log(LogSeverity::Information, "Failed to close stdin read handle");
    }

    if (!CloseHandle(this->stdInWrite))
    {
        this->logger.Log(LogSeverity::Information, "Failed to close stdin write handle");
    }

    if (!CloseHandle(this->stdOutRead))
    {
        this->logger.Log(LogSeverity::Information, "Failed to close stdout read handle");
    }

    if (!CloseHandle(this->stdOutWrite))
    {
        this->logger.Log(LogSeverity::Information, "Failed to close stdout write handle");
    }

    delete this->processInfo;
}

void ChildProcess::Start()
{
    this->logger.Log(LogSeverity::Information, "Initializing child process SECURITY_ATTRIBUTES.");

    // Set the bInheritHandle flag so pipe handles are inherited.
    SECURITY_ATTRIBUTES pipeSecurityAttributes;
    ZeroMemory(&pipeSecurityAttributes, sizeof(SECURITY_ATTRIBUTES));
    pipeSecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    pipeSecurityAttributes.bInheritHandle = true;
    pipeSecurityAttributes.lpSecurityDescriptor = nullptr;

    this->logger.Log(LogSeverity::Information, "Creating child process stdin/stdout pipes.");

    // Create stdin/stdout pipes.
    PHANDLE stdInRead = (PHANDLE)&this->stdInRead;
    PHANDLE stdInWrite = (PHANDLE)&this->stdInWrite;
    PHANDLE stdOutRead = (PHANDLE)&this->stdOutRead;
    PHANDLE stdOutWrite = (PHANDLE)&this->stdOutWrite;
    if (!CreatePipe(stdInRead, stdInWrite, &pipeSecurityAttributes, 0))
    {
        this->logger.Log(LogSeverity::Critical, "Failed to create child process stdin pipes");
        return;
    }
    if (!CreatePipe(stdOutRead, stdOutWrite, &pipeSecurityAttributes, 0))
    {
        this->logger.Log(LogSeverity::Critical, "Failed to create child process stdout pipes");
        return;
    }

    this->logger.Log(LogSeverity::Information, "Creating child process STARTUPINFO.");

    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = *stdOutWrite;
    startupInfo.hStdError = *stdOutWrite;
    startupInfo.hStdInput = *stdInRead;

    if (!CreateProcess(
        this->path.c_str(),
        nullptr,
        nullptr,
        nullptr,
        /* bInheritHandles */ TRUE,
        0,
        nullptr,
        nullptr,
        &startupInfo,
        (LPPROCESS_INFORMATION)this->processInfo))
    {
        this->logger.Log(LogSeverity::Critical, "Failed to start child process");
    }
}

void ChildProcess::Stop()
{
    this->logger.Log(LogSeverity::Critical, "Stopping child process " + this->path);

    if (this->GetIsRunning())
    {
        // Terminate process.
        LPPROCESS_INFORMATION processInfo = (LPPROCESS_INFORMATION)this->processInfo;

        if (processInfo->hProcess == INVALID_HANDLE_VALUE)
        {
            this->logger.Log(LogSeverity::Warning, "PROCESS_INFORMATION handle value is invalid. Skipping termination.");
        }
        else if (TerminateProcess(processInfo->hProcess, 0))
        {
            this->logger.Log(LogSeverity::Information, "TerminateProcess() succeeded.");

            if (!CloseHandle(processInfo->hProcess))
            {
                this->logger.Log(LogSeverity::Warning, "CloseHandle() failed on hProcess.");
            }

            if (!CloseHandle(processInfo->hThread))
            {
                this->logger.Log(LogSeverity::Warning, "CloseHandle() failed on hThread.");
            }
        }
        else
        {
            this->logger.Log(LogSeverity::Warning, "TerminateProcess() failed.");
        }
    }
}

bool ChildProcess::GetIsRunning()
{
    this->logger.Log(LogSeverity::Information, "Checking if child process is still running: " + this->path);

    LPPROCESS_INFORMATION processInfo = (LPPROCESS_INFORMATION)this->processInfo;
    DWORD exitCode = 0;
    if (!GetExitCodeProcess(processInfo->hProcess, &exitCode))
    {
        this->logger.Log(LogSeverity::Warning, "GetExitCodeProcess() failed");
        return false;
    }

    auto isRunning = exitCode == STILL_ACTIVE;
    if (isRunning)
    {
        this->logger.Log(LogSeverity::Information, "ChildProcess is still running.");
    }

    return isRunning;
}

const std::string ChildProcess::ReadStdOutLine()
{
    this->logger.Log(LogSeverity::Information, "Reading stdout line from: " + this->path);

    if (!this->GetIsRunning())
    {
        this->logger.Log(LogSeverity::Warning, "Child process has already exited");

        return std::string();
    }

    // TODO: dynamically grow buffer.
    char buffer[255] = { };
    DWORD bytesRead = 0;
    if (!ReadFile(this->stdOutRead, buffer, _countof(buffer), &bytesRead, nullptr))
    {
        this->logger.Log(LogSeverity::Warning, "Reading stdout line failed");
    }

    return std::string(buffer);
}

void ChildProcess::WriteStdInLine(const std::string& line)
{
    this->logger.Log(LogSeverity::Information, "Writing stdout line from: " + this->path);

    if (!this->GetIsRunning())
    {
        this->logger.Log(LogSeverity::Warning, "Child process has already exited");
        return;
    }

    // TODO: dynamically grow buffer.
    char buffer[255] = { };
    DWORD bytesRead = 0;
    std::string outputString = line + "\n";
    if (!WriteFile(this->stdInWrite, outputString.c_str(), outputString.size(), &bytesRead, nullptr))
    {
        this->logger.Log(LogSeverity::Warning, "Writing stdin line failed");
    }
}