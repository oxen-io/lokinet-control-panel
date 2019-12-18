#ifndef __LOKI_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_LOKINET_PROCESS_MANAGER_HPP__

#include <QObject>

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
    virtual bool startLokinetProcess() = 0;
    
    /**
     * Stop the lokinet process.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    virtual bool stopLokinetProcess() = 0;
    
    /**
     * Forcibly stop the lokinet process. Should only be called if a normal
     * stop is insufficient.
     *
     * @return false if an error occurs or the process is not running,
     *         true otherwise
     */
    virtual bool forciblyStopLokinetProcess() = 0;

    /**
     * Query the status of the process. This should query the OS for a realtime
     * status.
     *
     * @return an up-to-date ProcessStatus, possibly ProcessStatus::Unknown on
     *         error
     */
    virtual ProcessStatus queryProcessStatus() = 0;
};
 
#endif // __LOKI_LOKINET_PROCESS_MANAGER_HPP__
