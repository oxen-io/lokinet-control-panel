#ifndef __LOKI_WINDOWS_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_WINDOWS_LOKINET_PROCESS_MANAGER_HPP__

#include "LokinetProcessManager.hpp"

#include <QObject>

#ifdef Q_OS_WIN

/**
 * A Windows version of the Lokinet process manager.
 */
class WindowsLokinetProcessManager : public LokinetProcessManager
{
    Q_OBJECT

public:
    WindowsLokinetProcessManager();

protected:
    
    bool doStartLokinetProcess() override;
    bool doForciblyStopLokinetProcess() override;
    bool doGetProcessPid(int& pid) override;
    QString getDefaultBootstrapFileLocation() override;

};

#endif // Q_OS_WIN
#endif // __LOKI_WINDOWS_LOKINET_PROCESS_MANAGER_HPP__
