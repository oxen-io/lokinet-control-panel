#ifndef __LOKI_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_LOKINET_PROCESS_MANAGER_HPP__

#include <QObject>
#include <chrono>

/**
 * An abstract class for dealing with the lokinet process.
 */
class LokinetProcessManager : public QObject
{
    Q_OBJECT

public:

    enum class ProcessStatus
    {
        Unknown = 0,
        Starting,
        Running,
        Stopping,
        Stopped,
    };
    
    /**
     * Start the lokinet process.
     *
     * @return false if an error occurs or the process is already running,
     *         true otherwise
     */
    bool startLokinetProcess();
    
    /**
     * Stop the lokinet process.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    bool stopLokinetProcess();
    
    /**
     * Forcibly stop the lokinet process. Should only be called if a normal
     * stop is insufficient.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    bool forciblyStopLokinetProcess();

    /**
     * Query the status of the process. This should query the OS for a realtime
     * status.
     *
     * @return an up-to-date ProcessStatus, possibly ProcessStatus::Unknown on
     *         error
     */
    ProcessStatus queryProcessStatus();

    /**
     * Returns an appropriate platform-specific instance of this class.
     */
    static LokinetProcessManager* instance();

protected:
    
    /**
     * Subclasses should provide platform-specific means of starting the
     * lokinet process.
     */
    virtual bool doStartLokinetProcess() = 0;
    
    /**
     * Subclasses should provide platform-specific means of stopping the
     * lokinet process.
     */
    virtual bool doStopLokinetProcess() = 0;
    
    /**
     * Subclasses should provide platform-specific means of forcibly stopping
     * the lokinet process.
     */
    virtual bool doForciblyStopLokinetProcess() = 0;

    /**
     * Subclasses should provide platform-specific means of querying the pid
     * of the lokinet process (or 0 if there is no such process)
     */
    virtual bool doGetProcessPid(int& pid) = 0;

private:

    /**
     * Return the last known status if it is "recent"
     */
    ProcessStatus getLastKnownStatus();

    /**
     * Update the last known status and its timestamp
     */
    void setLastKnownStatus(ProcessStatus status);

    ProcessStatus m_lastKnownStatus = ProcessStatus::Unknown;
    std::chrono::system_clock::time_point m_lastStatusTime;
};
 
#endif // __LOKI_LOKINET_PROCESS_MANAGER_HPP__
