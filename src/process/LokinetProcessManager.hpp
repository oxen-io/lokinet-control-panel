#ifndef __LOKI_LOKINET_PROCESS_MANAGER_HPP__
#define __LOKI_LOKINET_PROCESS_MANAGER_HPP__

#include <QObject>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "../HttpClient.hpp"
#include "../LokinetApiClient.hpp"

/**
 * An abstract class for dealing with the lokinet process.
 *
 * This class provides a public interface for starting and stopping the lokinet
 * process and delegates to platform-specific subclasses for implementation of
 * actual process management.
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
     * Constructor
     */
    LokinetProcessManager();

    /**
     * Destructor
     */
    virtual ~LokinetProcessManager();
    
    /**
     * Start the lokinet process.
     *
     * @return false if an error occurs or the process is already running,
     *         true otherwise
     */
    bool startLokinetProcess();
    
    /**
     * Stop the lokinet process. Attempts to gracefully stop the procses by
     * using lokinet's 'llarp.admin.die' API call, which should cause the
     * process to stop gracefully.
     *
     * Note that this will not update m_didLaunchProcess as it doesn't know
     * whether or not the lokinet process exits gracefully. This may result in
     * an error condition being returned when calling stopLokinetIfWeStartedIt()
     * but this is harmless.
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
     * Stop the process in a managed way. The process will immediately be stopped
     * and will be given a short amount of time to gracefully exit, after which
     * it will be forcefully terminated.
     *
     * This will spawn a thread which will manage the termination process. Only
     * one thread may exist at a time; the function will return false if there
     * is already an outstanding thread.
     *
     * @return false if an error occurs or the process is not running or there
     *         is already managed stop process, true otherwise
     */
    bool managedStopLokinetProcess();

    /**
     * Invokes managedStopLokinetProcess() iff we launched the process in the first
     * place. Effectively this allows us to fully manage the lokinet process in the
     * normal case but won't touch the process on exit if it was launched by some
     * other mechanism.
     *
     * An exception is made for systemd mode, where we expect systemd to fully manage
     * the lifetime of lokinet.
     *
     * @param block determines whether or not we block until the managed process
     *        terminates.
     * @return false if any error occurs, true if we don't invoke
     *         managedStopLokinetProcess(), true if the process actually died
     *         within the timeout (MANAGED_KILL_WAIT)
     */
    virtual bool stopLokinetIfWeStartedIt(bool block = true);

    /**
     * Query the status of the process. This should query the OS for a realtime
     * status.
     *
     * @return an up-to-date ProcessStatus, possibly ProcessStatus::Unknown on
     *         error
     */
    ProcessStatus queryProcessStatus();

    /**
     * Download the lokinet bootstrap file and place it at the proper platform-
     * dependent location.
     *
     * This is a non-blocking/asynchronous call.
     *
     * @param url is the URL of the bootstrap file to download
     */
    using BootstrapCallback = std::function<void(int error, const std::string& msg)>;
    void downloadBootstrapFile(const std::string& url, BootstrapCallback callback);

    /**
     * Returns an appropriate platform-specific instance of this class.
     */
    static LokinetProcessManager* instance();

protected:
    
    /**
     * Subclasses should provide platform-specific means of starting the
     * lokinet process.
     *
     * @return true on success; false otherwise
     */
    virtual bool doStartLokinetProcess() = 0;
    
    /**
     * Subclasses may provide platform-specific means of stopping the
     * lokinet process. Note, however, that this has an "intelligent default"
     * which will use the 'llarp.admin.die' API call, which should cause
     * lokinet to gracefully exit.
     *
     * @return true on successful request to stop, false otherwise
     */
    virtual bool doStopLokinetProcess();
    
    /**
     * Subclasses should provide platform-specific means of forcibly stopping
     * the lokinet process.
     *
     * @return true on success; false otherwise
     */
    virtual bool doForciblyStopLokinetProcess() = 0;

    /**
     * Subclasses should provide platform-specific means of querying the pid
     * of the lokinet process (or 0 if there is no such process)
     *
     * @param (out) pid should be filled out with the pid of the lokinet process
     *              or 0 if there is no running process.
     * @return true on success; false otherwise
     */
    virtual bool doGetProcessPid(int& pid) = 0;

    /**
     * Subclasses should provide the platform-specific location of the bootstrap
     * file. This is used internally by downloadBootstrapFile() to save the down-
     * loaded file.
     *
     * The default is Qt's QStandardPaths::HomeLocation + "/.lokinet/bootstrap.signed"
     * which corresponds to ~/.lokinet/bootstrap.signed on Linux/MacOS.
     */
    virtual QString getDefaultBootstrapFileLocation();

private:

    /**
     * Return the last known status if it is "recent"
     */
    ProcessStatus getLastKnownStatus();

    /**
     * Update the last known status and its timestamp
     */
    void setLastKnownStatus(ProcessStatus status);

    HttpClient m_httpClient;
    LokinetApiClient m_apiClient;

    ProcessStatus m_lastKnownStatus = ProcessStatus::Unknown;
    std::chrono::system_clock::time_point m_lastStatusTime;

    std::mutex m_managedStopMutex; // prevents more than one concurrent call to managedStopLokinetProcess()
    std::atomic_bool m_managedThreadRunning;

    std::atomic_bool m_didLaunchProcess;
};
 
#endif // __LOKI_LOKINET_PROCESS_MANAGER_HPP__
