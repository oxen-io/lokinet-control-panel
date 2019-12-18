#include "LinuxLokinetProcessManager.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <chrono>

using namespace std::chrono_literals;

bool LinuxLokinetProcessManager::startLokinetProcess()
{
    // ensure that we're stopped and haven't recently started
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Stopped)
    {
        qDebug("Can't start lokinet process when status != Stopped");
        return false;
    }
    if (getLastKnownStatus() == ProcessStatus::Starting)
    {
        qDebug("lokinet process is already starting");
        return false;
    }

    // TODO: properly daemonize (and don't use system)
    int result = system("lokinet &");
    if (result)
    {
        qDebug("Failed to launch 'lokinet' process: %d", result);
        return false;
    }

    setLastKnownStatus(ProcessStatus::Starting);
    return true;
}

bool LinuxLokinetProcessManager::stopLokinetProcess()
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

    int result = system("killall lokinet");
    if (result)
    {
        qDebug("Failed to 'killall lokinet': %d", result);
        return false;
    }

    setLastKnownStatus(ProcessStatus::Stopping);
    return true;
}

bool LinuxLokinetProcessManager::forciblyStopLokinetProcess()
{
    // for forcibly kill, we don't care about 'last known status' but do
    // want to ensure that there is a running process to kill
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Running)
    {
        qDebug("Can't forcibly stop lokinet process when status != Running");
        return false;
    }

    int result = system("killall -9 lokinet");
    if (result)
    {
        qDebug("Failed to 'killall -9 lokinet': %d", result);
        return false;
    }

    setLastKnownStatus(ProcessStatus::Stopping);
    return true;
}

LokinetProcessManager::ProcessStatus LinuxLokinetProcessManager::queryProcessStatus()
{
    char buf[512];
    FILE *cmd_pipe = popen("pidof -s lokinet", "r");

    fgets(buf, 512, cmd_pipe);
    pid_t pid = strtoul(buf, NULL, 10);

    pclose( cmd_pipe ); 

    // TODO: check error
    return pid
        ? ProcessStatus::Running
        : ProcessStatus::Stopped;
}

LokinetProcessManager::ProcessStatus LinuxLokinetProcessManager::getLastKnownStatus()
{
    constexpr auto expiry = 60s;

    auto now = std::chrono::system_clock::now();
    if (now - m_lastStatusTime > expiry)
        return ProcessStatus::Unknown;
    else
        return m_lastKnownStatus;
}

void LinuxLokinetProcessManager::setLastKnownStatus(ProcessStatus status)
{
    m_lastKnownStatus = status;
    m_lastStatusTime = std::chrono::system_clock::now();
}
