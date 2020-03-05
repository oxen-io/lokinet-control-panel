#ifndef __LOKI_MACOS_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_MACOS_LOKINET_PROCESS_MANAGER_HPP__

#include "LokinetProcessManager.hpp"

#include <QObject>

#ifdef Q_OS_MACOS

/**
 * A MacOS version of the Lokinet process manager.
 */
class MacOSLokinetProcessManager : public LokinetProcessManager
{
    Q_OBJECT

public:
    MacOSLokinetProcessManager();
    ~MacOSLokinetProcessManager();

protected:

    bool doStartLokinetProcess() override;
    bool doForciblyStopLokinetProcess() override;
    bool doGetProcessPid(int& pid) override;

private:

    bool claimDNS();
    bool unclaimDNS();

    bool m_dnsClaimEnabled = true;
    bool m_dnsClaimed = false;
};

#endif // __LOKI_MACOS_LOKINET_PROCESS_MANAGER_HPP__
#endif // Q_OS_MACOS
