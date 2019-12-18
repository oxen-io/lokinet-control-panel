#include "LokinetProcessManager.hpp"

#if defined(Q_OS_LINUX)
#include "LinuxLokinetProcessManager.hpp"
LinuxLokinetProcessManager g_processManager;
#endif

LokinetProcessManager* LokinetProcessManager::instance()
{
    return &g_processManager;
}
