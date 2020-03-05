#include "LokinetProcessManager.hpp"

#include <chrono>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <sstream>
#include <memory>
#include <mutex>

using namespace std::literals::chrono_literals;

constexpr auto MANAGED_KILL_WAIT = 5s;

LokinetProcessManager::LokinetProcessManager()
    : m_managedThreadRunning(false)
    , m_didLaunchProcess(false)
{
}

LokinetProcessManager::~LokinetProcessManager()
{
}

bool LokinetProcessManager::startLokinetProcess()
{
    // ensure that we're stopped and haven't recently started
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Stopped)
    {
        qDebug("Can't start lokinet process when status != Stopped");
        qDebug() << "(status: " << (int)status << ")";
        return false;
    }
    if (getLastKnownStatus() == ProcessStatus::Starting)
    {
        qDebug("lokinet process is already starting");
        return false;
    }

    bool success = doStartLokinetProcess();
    if (! success)
    {
        qDebug("Failed to launch 'lokinet' process");
        return false;
    }

    m_didLaunchProcess = true;

    setLastKnownStatus(ProcessStatus::Starting);
    return true;
}

bool LokinetProcessManager::stopLokinetProcess()
{
    // ensure that we're running and haven't recently stopped
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Running)
    {
        qDebug("Can't stop lokinet process when status != Running");
        return false;
    }
    if (getLastKnownStatus() == ProcessStatus::Stopping)
    {
        qDebug("lokinet process is already stopping");
        return false;
    }

    bool success = m_apiClient.llarpAdminDie([](QNetworkReply* reply) {
        qDebug() << "llarp.admin.die response: " << reply->readAll();
    });
    if (!success)
    {
        qDebug("Failed to stop lokinet process with llarp.admin.die API call");
        return false;
    }

    // note that we don't touch m_didLaunchProcess here because we don't
    // know whether or not lokinet will gracefully exit

    setLastKnownStatus(ProcessStatus::Stopping);
    return true;
}

bool LokinetProcessManager::forciblyStopLokinetProcess()
{
    // for forcibly kill, we don't care about 'last known status' but do
    // want to ensure that there is a running process to kill
    ProcessStatus status = queryProcessStatus();
    if (status != ProcessStatus::Running)
    {
        qDebug("Can't forcibly stop lokinet process when status != Running");
        return false;
    }

    bool success = doForciblyStopLokinetProcess();
    if (!success)
    {
        qDebug("Failed to forcibly stop lokinet process");
        return false;
    }

    m_didLaunchProcess = false;
    setLastKnownStatus(ProcessStatus::Stopping);

    return true;
}

bool LokinetProcessManager::managedStopLokinetProcess()
{
    if (not isGracefulKillSupported())
    {
        qDebug("Platform doesn't support graceful kill, death will be swift and merciless");
        if (not doForciblyStopLokinetProcess())
        {
            qDebug("doForciblyStopLokinetProcess() failed, good luck");
            return false;
        }
        return true;
    }

    std::lock_guard<std::mutex> guard(m_managedStopMutex);

    if (m_managedThreadRunning)
    {
        qDebug("Cannot create a managed stop thread; one already exists");
        return false;
    }

    if (queryProcessStatus() != ProcessStatus::Running)
    {
        // TODO: should tolerate status == Starting
        qDebug("Cannot create a managed stop thread when lokinet is not running");
        return false;
    }

    m_managedThreadRunning = true;

    std::thread t([this]() {
        if (not stopLokinetProcess())
        {
            m_managedThreadRunning = false;
            qDebug("stopLokinetProcess() failed in managed stop thread");
            return;
        }

        qDebug() << "Waiting for "
                 << std::chrono::milliseconds(MANAGED_KILL_WAIT).count()
                 << "ms for graceful lokinet exit...";

        // check status frequently until lokinet dies or we timeout
        auto start = std::chrono::system_clock::now();
        while ((std::chrono::system_clock::now() - start) < MANAGED_KILL_WAIT
                && queryProcessStatus() == ProcessStatus::Running)
        {
            std::this_thread::sleep_for(50ms);
        }

        if (queryProcessStatus() == ProcessStatus::Running)
        {
            qDebug("lokinet did not exit gracefully, forcefully stopping it...");
            doForciblyStopLokinetProcess();
        }
        else
        {
            qDebug("lokinet exited gracefully");
        }

        m_didLaunchProcess = false;
        m_managedThreadRunning = false;

    });
    t.detach();

    return true;
}

bool LokinetProcessManager::stopLokinetIfWeStartedIt(bool block)
{
    if (m_didLaunchProcess)
    {
        qDebug("stopLokinetIfWeStartedIt() killing lokinet process since we launched it");
        bool success = managedStopLokinetProcess();
        if (not success)
        {
            qDebug("managedStopLokinetProcess() failed inside stopLokinetIfWeStartedIt()");
            return false;
        }

        // check status frequently until lokinet dies or we timeout (where timeout
        // is slightly longer here than in managedStopLokinetProcess())
        if (block)
        {
            // wait slightly longer than managedStopLokinetProcess() to account for other
            // calls that it makes, otherwise it might not make its force-kill call
            constexpr auto timeout = MANAGED_KILL_WAIT + 1s;

            auto start = std::chrono::system_clock::now();
            while ((std::chrono::system_clock::now() - start) < timeout
                    && queryProcessStatus() == ProcessStatus::Running)
            {
                std::this_thread::sleep_for(50ms);
            }

            return (queryProcessStatus() != ProcessStatus::Running);
        }
    }

    return true;
}

void LokinetProcessManager::downloadBootstrapFile(const std::string& url, BootstrapCallback callback)
{
    m_httpClient.get(url, [=](QNetworkReply* reply) {

        std::stringstream ss;

        // inspect error
        // https://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum
        auto error = reply->error();
        if (error)
        {
            // if < 200, we didn't talk to server
            if (error < 200)
                ss << "Failed to contact server: " << error;
            else
                ss << "Server replied: " << error;
        }
        else
        {

            QString filepath = getDefaultBootstrapFileLocation();

            // TODO: should be done in different thread?
            QFile file(filepath);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.close();

            ss << "Bootstrap file written to " << filepath.toStdString();
        }

        callback(error, ss.str());
    });
}

LokinetProcessManager::ProcessStatus LokinetProcessManager::queryProcessStatus()
{
    int pid = 0;
    bool success = doGetProcessPid(pid);
    if (! success)
    {
        qDebug("Failed to get lokinet pid");
        return ProcessStatus::Unknown;
    }
    return pid
        ? ProcessStatus::Running
        : ProcessStatus::Stopped;
}

QString LokinetProcessManager::getDefaultBootstrapFileLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        + "/.lokinet/bootstrap.signed";
}

LokinetProcessManager::ProcessStatus LokinetProcessManager::getLastKnownStatus()
{
    constexpr auto expiry = 60s;

    auto now = std::chrono::system_clock::now();
    if (now - m_lastStatusTime > expiry)
        return ProcessStatus::Unknown;
    else
        return m_lastKnownStatus;
}

void LokinetProcessManager::setLastKnownStatus(ProcessStatus status)
{
    m_lastKnownStatus = status;
    m_lastStatusTime = std::chrono::system_clock::now();
}

// define a global instance of the platform-specific process manager at
// compile-time

#if defined(Q_OS_WIN)
#include "WindowsLokinetProcessManager.hpp"
WindowsLokinetProcessManager g_processManager;

#elif defined(Q_OS_MACOS)
#include "MacOSLokinetProcessManager.hpp"
MacOSLokinetProcessManager g_processManager;

// tested: Linux, NetBSD, FreeBSD, Solaris
// netbsd dev reimplemented pkill and all the other
// FOSS kernels ship it
#else
#include "LinuxLokinetProcessManager.hpp"
LinuxLokinetProcessManager g_processManager;

#endif

LokinetProcessManager* LokinetProcessManager::instance()
{
    return &g_processManager;
}
