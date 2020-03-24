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

bool invoke(const QString& cmd, const QStringList& args)
{
    int result = QProcess::execute(cmd, args);
    if (result)
    {
        qDebug() << "Failed to invoke " << cmd;
        qDebug() << "    Args:";
        for (const auto arg : args)
        {
            qDebug() << "        " << arg;
        }
        return false;
    }

    return true;
}

bool SystemdLokinetProcessManager::doStopLokinetProcess()
{
    return invoke("systemctl", { "--no-block", "stop", "lokinet.service" });
}

bool SystemdLokinetProcessManager::doStartLokinetProcess()
{
    return invoke("systemctl", { "--no-block", "start", "lokinet.service" });
}

bool SystemdLokinetProcessManager::doForciblyStopLokinetProcess()
{
    // systemd's "stop" is a managed stop -- it will do its own forceful kill
    return invoke("systemctl", { "--no-block", "stop", "lokinet.service" });
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
