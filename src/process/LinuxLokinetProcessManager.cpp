#include "LinuxLokinetProcessManager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QDebug>

#include <chrono>

using namespace std::chrono_literals;

bool LinuxLokinetProcessManager::startLokinetProcess()
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

    bool success = QProcess::startDetached("lokinet");
    if (! success)
    {
        qDebug("Failed to launch 'lokinet' process");
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

    QStringList args = { "lokinet" };
    int result = QProcess::execute("killall", args);
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

    QStringList args = { "-9", "lokinet" };
    int result = QProcess::execute("killall", args);
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

    QProcess proc;
    proc.setProgram("pidof");
    proc.setArguments({"-s", "lokinet"});
    proc.start();
    bool success = proc.waitForFinished(5000);
    if (!success)
    {
        qDebug("Could not exec pidof");
        return ProcessStatus::Unknown;
    }

    QString output = QString(proc.readAllStandardOutput());
    if (output.isEmpty())
    {
        return ProcessStatus::Stopped;
    }

    pid_t pid = output.toInt(&success);
    if (!success)
    {
        qDebug() << "Unrecognized 'pidof' output: " << output;
        return ProcessStatus::Unknown;
    }

    qDebug() << "lokinet pid: " << pid;

    return ProcessStatus::Running;
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
