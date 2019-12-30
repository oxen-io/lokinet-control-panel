#ifdef Q_OS_LINUX
#ifndef __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__

#include "LokinetProcessManager.hpp"

#include <QObject>

/**
 * A Linux version of the Lokinet process manager.
 */
class LinuxLokinetProcessManager : public LokinetProcessManager
{
    Q_OBJECT

protected:
    
    bool doStartLokinetProcess() override;
    bool doStopLokinetProcess() override;
    bool doForciblyStopLokinetProcess() override;
    bool doGetProcessPid(int& pid) override;

};

#endif // __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__
#endif // Q_OS_LINUX
