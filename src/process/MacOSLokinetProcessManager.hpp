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

protected:

    bool doStartLokinetProcess() override;
    bool doStopLokinetProcess() override;
    bool doForciblyStopLokinetProcess() override;
    bool doGetProcessPid(int& pid) override;
    QString getDefaultBootstrapFileLocation() override;

};

#endif // __LOKI_MACOS_LOKINET_PROCESS_MANAGER_HPP__
#endif // Q_OS_MACOS
