#include "MacOSLokinetProcessManager.hpp"

#include <Security/Authorization.h>

#include <QProcess>
#include <QDebug>

#ifdef Q_OS_MACOS

#include "CoreFoundation/CoreFoundation.h"
#include "CoreFoundation/CFBundle.h"

#include <sys/stat.h> // for stat()
#include <libgen.h> // for dirname()
#include <sys/param.h> // for MAXPATHLEN
#include <unistd.h> // for getcwd()

#define LOKINET_BINARY_NAME    "lokinet"

// these paths are all relative to the app bundle root
#define DNS_CLAIM_FILEPATH     "./dns_claim.sh"
#define DNS_UNCLAIM_FILEPATH   "./dns_unclaim.sh"

#define DISABLE_DNS_FILE       "disable_auto_dns"

char *getExecutablePath() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef executeablesURL = CFBundleCopyExecutableURL(mainBundle);
    char path[PATH_MAX];
    if (not CFURLGetFileSystemRepresentation(executeablesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        qDebug("Couldn't getExecutablePath, good luck");
    }
    CFRelease(executeablesURL);
    return strdup(dirname(path));
}

char *getResourcesPath() {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (not CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        qDebug("Couldn't getResourcesPath, good luck");
    }
    CFRelease(resourcesURL);

    return strdup(path);
}

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

MacOSLokinetProcessManager::MacOSLokinetProcessManager()
{
    char *resourcePath = getResourcesPath();
    qDebug() << "Resource Path: " << resourcePath;
    std::string dtd_path(resourcePath);
    free(resourcePath);
    dtd_path.append(std::string(DISABLE_DNS_FILE));
    struct stat buffer;
    if (stat(dtd_path.c_str(), & buffer) == 0) {
      qDebug("DNS Claim disabled");
      m_dnsClaimEnabled = false;
    } else {
      qDebug("DNS Claim enabled");
    }
    char *execPath = getExecutablePath();
    chdir(execPath);
    qDebug() << "Executable Path: " << execPath;
    free(execPath);
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
    if (not m_dnsClaimEnabled) return true;

    char path[MAXPATHLEN];
    getwd(path);
    qDebug() << "Current Path: " << path;
    int result = system(DNS_CLAIM_FILEPATH);
    if (result)
        qDebug() << "warning: failed to claim dns: " << result;
    else
        m_dnsClaimed = true;

    return (result == 0);
}

bool MacOSLokinetProcessManager::unclaimDNS()
{
    if (not m_dnsClaimEnabled) return true;
    int result = system(DNS_UNCLAIM_FILEPATH);
    if (result)
        qDebug() << "warning: failed to unclaim dns: " << result;
    else
        m_dnsClaimed = false;

    return (result == 0);
}

bool MacOSLokinetProcessManager::doStartLokinetProcess()
{
    bool success = sudo(LOKINET_BINARY_NAME, NULL);
    if (not success) {
        qDebug("failed to launch lokinet via AuthorizationExecuteWithPrivileges");
        return false;
    }

    success = claimDNS();
    if (not success)
        qDebug("dns claim failed");


    return success;
}

bool MacOSLokinetProcessManager::doStopLokinetProcess()
{
    const char* args[] = {LOKINET_BINARY_NAME, NULL};
    success = sudo("/usr/bin/killall", args);
    if (not success) {
        qDebug("failed to kill lokinet via AuthorizationExecuteWithPrivileges");
        return false;
    }
    bool success = unclaimDNS();
    if (not success)
        qDebug("dns unclaim failed, killing lokinet anyway");

    return success;
}

bool MacOSLokinetProcessManager::doForciblyStopLokinetProcess()
{
    const char* args[] = {"-9", LOKINET_BINARY_NAME, NULL};
    bool success = sudo("/usr/bin/killall", args);
    if (not success)
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
