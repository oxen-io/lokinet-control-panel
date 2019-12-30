#ifdef Q_OS_LINUX

#include "LinuxLokinetProcessManager.hpp"

#include <QProcess>
#include <QDebug>

bool LinuxLokinetProcessManager::doStartLokinetProcess()
{
    bool success = QProcess::startDetached("lokinet");
    if (! success)
        qDebug("QProcess::startDetached() failed");

    return success;
}

bool LinuxLokinetProcessManager::doStopLokinetProcess()
{
    QStringList args = { "lokinet" };
    int result = QProcess::execute("killall", args);
    if (result)
    {
        qDebug("Failed to 'killall lokinet': %d", result);
        return false;
    }

    return true;
}

bool LinuxLokinetProcessManager::doForciblyStopLokinetProcess()
{
    QStringList args = { "-9", "lokinet" };
    int result = QProcess::execute("killall", args);
    if (result)
    {
        qDebug("Failed to 'killall -9 lokinet': %d", result);
        return false;
    }

    return true;
}

bool LinuxLokinetProcessManager::doGetProcessPid(int& pid)
{
    QProcess proc;
    proc.setProgram("pidof");
    proc.setArguments({"-s", "lokinet"});
    proc.start();
    bool success = proc.waitForFinished(5000);
    if (!success)
    {
        qDebug("Could not exec pidof");
        return false;
    }

    QString output = QString(proc.readAllStandardOutput());
    if (output.isEmpty())
    {
        qDebug("lokinet is not running (pid = 0)");
        pid = 0;
        return true;
    }

    int tmp = output.toInt(&success);
    if (!success)
    {
        qDebug() << "Unrecognized 'pidof' output: " << output;
        return false;
    }

    pid = tmp;
    qDebug("lokinet pid: %d", pid);
    return true;
}

#endif // Q_OS_LINUX
