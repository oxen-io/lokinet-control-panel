#include "MacOSLokinetProcessManager.hpp"

#include <Security/Authorization.h>

#include <QProcess>
#include <QDebug>
#include <QFile>

#ifdef Q_OS_MACOS

// This magic file causes launchd to suspend management of lokinet.
// Launchd will not automatically lauch lokinet so long as this file exists.
//
// However, it will not try to kill lokinet if it is already running when this
// file is created, so to stop lokinet, we need to:
// 1) create this file
// 2) kill lokinet
//
// In order to start lokinet, we simply need to remove the file -- launchd will
// immediately notice and spawn lokinet.
constexpr auto suspendFile = "/var/lib/lokinet/suspend-launchd-service";

bool MacOSLokinetProcessManager::doStartLokinetProcess()
{
    // starting lokinet requires removal of our magicFile
    QFile magicFile(suspendFile);
    if (magicFile.exists()) {
        qDebug() << "attempting to remove magic file " << suspendFile;
        if (not magicFile.remove()) {
            qDebug() << "Warning: failed to remove magic file " << suspendFile; 
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "magic file " << suspendFile << " didn't exist, lokinet shouldn't be running";
        return true;
    }
}

bool MacOSLokinetProcessManager::doStopLokinetProcess()
{
    // starting lokinet requires creating our magicFile
    QFile magicFile(suspendFile);
    if (magicFile.exists()) {
        qDebug() << "magic file " << suspendFile << " already exists, lokinet should not be running";
    } else {
        qDebug() << "no magic file " << suspendFile << ", attempting to create...";

        // open() will attempt to create the file if it doesn't exist (when invoked in ReadWrite mode)
        if (not magicFile.open(QIODevice::ReadWrite))
        {
            qDebug() << "failed to touch magic file " << suspendFile;
            return false;
        }
    }

    // now that we have created our magic file, we need to kill lokinet (which we can do by
    // simply delegating to our superclass)
    return LokinetProcessManager::doStopLokinetProcess();
}

bool MacOSLokinetProcessManager::doForciblyStopLokinetProcess()
{
    // TODO: we would need root privileges to do this. for now, just try the
    //       graceful approach
    return doStopLokinetProcess();
}

bool MacOSLokinetProcessManager::doGetProcessPid(int& pid)
{
    QProcess proc;
    proc.setProgram("pgrep");
    proc.setArguments({"^lokinet$"}); // strictly match lokinet, e.g. not lokinet-gui, etc.
    proc.start();
    bool success = proc.waitForFinished(5000);
    if (not success)
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
    if (not success)
    {
        qDebug() << "Unrecognized 'pidof' output: " << output;
        return false;
    }

    pid = tmp;
    qDebug("lokinet pid: %d", pid);
    return true;
}

#endif // Q_OS_MACOS
