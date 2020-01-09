#include "MacOSLokinetProcessManager.hpp"

#include <Security/Authorization.h>

#include <QProcess>
#include <QDebug>

#ifdef Q_OS_MACOS

#define LOKINET_BINARY_NAME    "lokinet"

// these paths are all relative to the app bundle root
#define BINARY_PATH            "Contents/MacOS/"
#define DNS_CLAIM_FILEPATH     BINARY_PATH "/dns_claim.sh"
#define DNS_UNCLAIM_FILEPATH   BINARY_PATH "/dns_unclaim.sh"
#define LOKINET_FILEPATH       BINARY_PATH "/" LOKINET_BINARY_NAME

#define RESOURCES_PATH         "Contents/Resources/"
#define DISABLE_DNS_FILE       RESOURCES_PATH "/disable_auto_dns"

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

MacOSLokinetProcessManager::~MacOSLokinetProcessManager()
{
    if (m_dnsClaimed)
    {
        unclaimDNS();
    }
}

bool MacOSLokinetProcessManager::claimDNS()
{
    int result = system(DNS_CLAIM_FILEPATH);
    if (result)
        qDebug() << "warning: failed to claim dns: " << result;
    else
        m_dnsClaimed = true;

    return (result == 0);
}

bool MacOSLokinetProcessManager::unclaimDNS()
{
    int result = system(DNS_UNCLAIM_FILEPATH);
    if (result)
        qDebug() << "warning: failed to unclaim dns: " << result;
    else
        m_dnsClaimed = false;

    return (result == 0);
}

bool MacOSLokinetProcessManager::doStartLokinetProcess()
{
    bool success = claimDNS();
    if (! success)
        qDebug("dns claim failed, starting lokinet anyway");

    success = sudo(LOKINET_FILEPATH, {NULL});
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doStopLokinetProcess()
{
    bool success = unclaimDNS();
    if (! success)
        qDebug("dns unclaim failed, killing lokinet anyway");

    const char* args[] = {LOKINET_BINARY_NAME, NULL};
    success = sudo("/usr/bin/killall", args);
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doForciblyStopLokinetProcess()
{
    const char* args[] = {"-9", LOKINET_BINARY_NAME, NULL};
    bool success = sudo("/usr/bin/killall", args);
    if (! success)
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");

    return success;
}

bool MacOSLokinetProcessManager::doGetProcessPid(int& pid)
{
    QProcess proc;
    proc.setProgram("pgrep");
    proc.setArguments({LOKINET_BINARY_NAME});
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
