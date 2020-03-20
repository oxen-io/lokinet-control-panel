#include "SystemdLokinetProcessManager.hpp"

#include <QProcess>
#include <QDebug>

#if defined(SYSTEMD)

enum class SystemdStatus {
    unknown,
    active,
    reloading,
    inactive,
    failed,
    activating,
    deactivating,
    maintenance
};
SystemdStatus fromString(const QString& str) {
    if (str == "active") return SystemdStatus::active;
    if (str == "reloading") return SystemdStatus::reloading;
    if (str == "inactive") return SystemdStatus::inactive;
    if (str == "failed") return SystemdStatus::failed;
    if (str == "activating") return SystemdStatus::activating;
    if (str == "deactivating") return SystemdStatus::deactivating;
    if (str == "maintenance") return SystemdStatus::maintenance;
    return SystemdStatus::unknown;
}

bool SystemdLokinetProcessManager::doStartLokinetProcess()
{
    QStringList args = { "--noblock", "start", "lokinet.service" };
    int result = QProcess::execute("systemctl", args);
    if (result)
    {
        qDebug("Failed to 'systemctl start lokinet': %d", result);
        return false;
    }

    return true;
}

bool SystemdLokinetProcessManager::doForciblyStopLokinetProcess()
{
    // note that this will do its own graceful attempt before doing a forceful kill.
    // "kill" instead of "stop" should perform an immediate, forceful kill
    QStringList args = { "stop", "lokinet.service" };
    int result = QProcess::execute("systemctl", args);
    if (result)
    {
        qDebug("Failed to 'systemctl stop lokinet': %d", result);
        return false;
    }

    return true;
}

bool SystemdLokinetProcessManager::doGetProcessPid(int& pid)
{
    // we don't actually get the pid here, because we don't really need it. so we set pid to 1 to
    // indicate that lokinet is running or 0 to indicate that it is not.

    QProcess proc;
    proc.setProgram("systemctl");
    proc.setArguments({"is-active", "lokinet.service"});
    proc.start();
    bool success = proc.waitForFinished(1000);
    if (!success)
    {
        qDebug("Could not exec systemctl");
        return false;
    }

    QString output = QString(proc.readAllStandardOutput());
    qDebug() << "systemd reported status: " << output;
    SystemdStatus status = fromString(output.trimmed());

    switch (status) {
        case SystemdStatus::unknown:
            qDebug() << "failed to parse status";
            return false;

        case SystemdStatus::active:
        case SystemdStatus::activating:
        case SystemdStatus::reloading:
        case SystemdStatus::deactivating:
        case SystemdStatus::maintenance:
            qDebug() << "evaluated as running";
            pid = 1;
            break;

        case SystemdStatus::inactive:
        case SystemdStatus::failed:
        default:
            qDebug() << "evaluated as not running";
            pid = 0;
            break;
    }

    return true;
}

#endif // defined(SYSTEMD)
