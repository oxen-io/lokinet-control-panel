#include "MacOSLokinetProcessManager.hpp"

#include <Security/Authorization.h>

#include <QProcess>
#include <QDebug>

#ifdef Q_OS_MACOS

// utility to run a process as root
// Note that 'args' should be NULL-terminated
bool sudo(const std::string& cmd, const char* args[])
{
    // TODO: AuthorizationExecuteWithPrivileges is deprecated in favor of using
    // AuthorizationServices

    AuthorizationRef authorizationRef;
    OSStatus status;
    status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment,
                                 kAuthorizationFlagDefaults, &authorizationRef);

    FILE* pipe = NULL;
    status = AuthorizationExecuteWithPrivileges(
            authorizationRef,
            cmd.c_str(),
            kAuthorizationFlagDefaults,
            (char *const  _Nonnull * _Nonnull)args,
            &pipe);

    return (status == errAuthorizationSuccess);
}

bool MacOSLokinetProcessManager::doStartLokinetProcess()
{
    bool success = sudo("/usr/local/bin/lokinet", {NULL});
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doStopLokinetProcess()
{
    const char* args[] = {"lokinet", NULL};
    bool success = sudo("/usr/bin/killall", args);
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doForciblyStopLokinetProcess()
{
    const char* args[] = {"-9", "lokinet", NULL};
    bool success = sudo("/usr/bin/killall", args);
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doGetProcessPid(int& pid)
{
    QProcess proc;
    proc.setProgram("pgrep");
    proc.setArguments({"lokinet"});
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

#endif // Q_OS_MACOS
