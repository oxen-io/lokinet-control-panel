#include "PlatformDetails.hpp"

// PlatformDetails::isWindows
Q_INVOKABLE bool PlatformDetails::isWindows() {
#if defined(Q_OS_WINDOWS)
	return true;
#else
	return false;
#endif

}

// PlatformDetails::isLinux
Q_INVOKABLE bool PlatformDetails::isLinux() {
#if defined(Q_OS_LINUX)
	return true;
#else
	return false;
#endif
}

// PlatformDetails::isMacOS
Q_INVOKABLE bool PlatformDetails::isMacOS() {
#if defined(Q_OS_MACOS)
	return true;
#else
	return false;
#endif
}

// PlatformDetails::isDebug
Q_INVOKABLE bool PlatformDetails::isDebug() {
#if defined(QT_DEBUG)
	return true;
#else
	return false;
#endif
}

Q_INVOKABLE bool PlatformDetails::startLokinetProcess() {
	return LokinetProcessManager::instance()->startLokinetProcess();
}

Q_INVOKABLE bool PlatformDetails::stopLokinetProcess() {
	return LokinetProcessManager::instance()->stopLokinetProcess();
}

Q_INVOKABLE bool PlatformDetails::forciblyStopLokinetProcess() {
	return LokinetProcessManager::instance()->forciblyStopLokinetProcess();
}

Q_INVOKABLE bool PlatformDetails::managedStopLokinetProcess() {
	return LokinetProcessManager::instance()->managedStopLokinetProcess();
}

Q_INVOKABLE bool PlatformDetails::isLokinetRunning() {
	auto status = LokinetProcessManager::instance()->queryProcessStatus();
	return (status == LokinetProcessManager::ProcessStatus::Running);
}

