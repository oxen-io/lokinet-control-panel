#include "LokinetProcessManager.hpp"

#include <chrono>
#include <QDebug>
#include <memory>
#include <mutex>

using namespace std::literals::chrono_literals;

constexpr auto MANAGED_KILL_WAIT = 5s;

LokinetProcessManager::LokinetProcessManager()
    : m_managedThreadRunning(false)
{
}

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

bool LokinetProcessManager::managedStopLokinetProcess()
{
    std::lock_guard<std::mutex> guard(m_managedStopMutex);

    if (m_managedThreadRunning)
    {
        qDebug("Cannot create a managed stop thread; one already exists");
        return false;
    }

    if (queryProcessStatus() != ProcessStatus::Running)
    {
        // TODO: should tolerate status == Starting
        qDebug("Cannot create a managed stop thread when lokinet is not running");
        return false;
    }

    m_managedThreadRunning = true;

    std::thread t([this]() {
        if (not stopLokinetProcess())
        {
            m_managedThreadRunning = false;
            qDebug("stopLokinetProcess() failed in managed stop thread");
            return;
        }

        qDebug() << "Waiting for "
                 << std::chrono::milliseconds(MANAGED_KILL_WAIT).count()
                 << "ms for graceful lokinet exit...";
        std::this_thread::sleep_for(MANAGED_KILL_WAIT);

        if (queryProcessStatus() == ProcessStatus::Running)
        {
            qDebug("lokinet did not exit gracefully, forcefully stopping it...");
            doForciblyStopLokinetProcess();
        }
        else
        {
            qDebug("lokinet exited gracefully");
        }

        m_managedThreadRunning = false;

    });
    t.detach();

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
#if defined(Q_OS_LINUX)
#include "LinuxLokinetProcessManager.hpp"
LinuxLokinetProcessManager g_processManager;

#elif defined(Q_OS_WIN)
#include "WindowsLokinetProcessManager.hpp"
WindowsLokinetProcessManager g_processManager;

#elif defined(Q_OS_MACOS)
#include "MacOSLokinetProcessManager.hpp"
MacOSLokinetProcessManager g_processManager;

#endif

LokinetProcessManager* LokinetProcessManager::instance()
{
    return &g_processManager;
}
