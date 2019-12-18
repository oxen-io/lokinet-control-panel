#ifndef __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__

#include "LokinetProcessManager.hpp"

#include <chrono>

#include <QObject>

/**
 * A Linux version of the Lokinet process manager.
 */
class LinuxLokinetProcessManager : public LokinetProcessManager
{
    Q_OBJECT

public:
    
    /**
     * Start the lokinet process.
     *
     * @return false if an error occurs or the process is already running,
     *         true otherwise
     */
    bool startLokinetProcess() override;
    
    /**
     * Stop the lokinet process.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    bool stopLokinetProcess() override;
    
    /**
     * Forcibly stop the lokinet process. Should only be called if a normal
     * stop is insufficient.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    bool forciblyStopLokinetProcess() override;

    /**
     * Query the realtime status of the process.
     *
     * @return an up-to-date ProcessStatus, or ProcessStatus::Unknown on error
     */
    ProcessStatus queryProcessStatus() override;

protected:

    /**
     * Return the last known status if it is "recent"
     */
    ProcessStatus getLastKnownStatus();

    /**
     * Update the last known status and its timestamp
     */
    void setLastKnownStatus(ProcessStatus status);

private:

    ProcessStatus m_lastKnownStatus = ProcessStatus::Unknown;
    std::chrono::system_clock::time_point m_lastStatusTime;


};
 
#endif // __LOKI_LINUX_LOKINET_PROCESS_MANAGER_HPP__
