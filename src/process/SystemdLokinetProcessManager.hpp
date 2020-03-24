#pragma once

#include "LokinetProcessManager.hpp"

#include <QObject>

#if defined(SYSTEMD)

/**
 * A Systemd version of the Lokinet process manager.
 */
class SystemdLokinetProcessManager : public LokinetProcessManager
{
    Q_OBJECT

protected:
    
    bool stopLokinetIfWeStartedIt(bool) {return true;};
    bool doStopLokinetProcess() override;
    bool doStartLokinetProcess() override;
    bool doForciblyStopLokinetProcess() override;
    bool doGetProcessPid(int& pid) override;

};

#endif // defined(SYSTEMD)
