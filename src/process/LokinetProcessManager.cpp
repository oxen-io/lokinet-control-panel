#include "LokinetProcessManager.hpp"

#include <chrono>
#include <QDebug>

using namespace std::literals::chrono_literals;

bool LokinetProcessManager::startLokinetProcess()
{
    // ensure that we're stopped and haven't recently started
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Stopped)
    {
        qDebug("Can't start lokinet process when status != Stopped");
        qDebug() << "(status: " << (int)status << ")";
        return false;
    }
    if (getLastKnownStatus() == ProcessStatus::Starting)
    {
        qDebug("lokinet process is already starting");
        return false;
    }

    bool success = doStartLokinetProcess();
    if (! success)
    {
        qDebug("Failed to launch 'lokinet' process");
        return false;
    }

    setLastKnownStatus(ProcessStatus::Starting);
    return true;
}

bool LokinetProcessManager::stopLokinetProcess()
{
    // ensure that we're running and haven't recently stopped
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Running)
    {
        qDebug("Can't stop lokinet process when status != Running");
        return false;
    }
    if (getLastKnownStatus() == ProcessStatus::Stopping)
    {
        qDebug("lokinet process is already stopping");
        return false;
    }

    bool success = doStopLokinetProcess();
    if (!success)
    {
        qDebug("Failed to stop lokinet process");
        return false;
    }

    setLastKnownStatus(ProcessStatus::Stopping);
    return true;
}

bool LokinetProcessManager::forciblyStopLokinetProcess()
{
    // for forcibly kill, we don't care about 'last known status' but do
    // want to ensure that there is a running process to kill
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Running)
    {
        qDebug("Can't forcibly stop lokinet process when status != Running");
        return false;
    }

    bool success = doForciblyStopLokinetProcess();
    if (!success)
    {
        qDebug("Failed to forcibly stop lokinet process");
        return false;
    }

    setLastKnownStatus(ProcessStatus::Stopping);
    return true;
}

LokinetProcessManager::ProcessStatus LokinetProcessManager::queryProcessStatus()
{
    int pid = 0;
    bool success = doGetProcessPid(pid);
    if (! success)
    {
        qDebug("Failed to get lokinet pid");
        return ProcessStatus::Unknown;
    }
    return pid
        ? ProcessStatus::Running
        : ProcessStatus::Stopped;
}

LokinetProcessManager::ProcessStatus LokinetProcessManager::getLastKnownStatus()
{
    constexpr auto expiry = 60s;

    auto now = std::chrono::system_clock::now();
    if (now - m_lastStatusTime > expiry)
        return ProcessStatus::Unknown;
    else
        return m_lastKnownStatus;
}

void LokinetProcessManager::setLastKnownStatus(ProcessStatus status)
{
    m_lastKnownStatus = status;
    m_lastStatusTime = std::chrono::system_clock::now();
}

// define a global instance of the platform-specific process manager at
// compile-time

#if defined(Q_OS_WIN)
#include "WindowsLokinetProcessManager.hpp"
WindowsLokinetProcessManager g_processManager;

#elif defined(Q_OS_MACOS)
#include "MacOSLokinetProcessManager.hpp"
MacOSLokinetProcessManager g_processManager;

#else
#include "LinuxLokinetProcessManager.hpp"
LinuxLokinetProcessManager g_processManager;

#endif

LokinetProcessManager* LokinetProcessManager::instance()
{
    return &g_processManager;
}
