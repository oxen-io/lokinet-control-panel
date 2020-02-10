#ifndef __LOKI_PLATFORM_DETAILS_HPP__
#define __LOKI_PLATFORM_DETAILS_HPP__

#include <QObject>
#include <QPoint>
#include <QJSValue>

#include "process/LokinetProcessManager.hpp"

/**
 * A class for exposing platform-specific details to QML
 */
class PlatformDetails : public QObject
{
    Q_OBJECT

public:

    Q_INVOKABLE static bool isWindows();
    Q_INVOKABLE static bool isLinux();
    Q_INVOKABLE static bool isMacOS();

    Q_INVOKABLE static bool isDebug();

    /**
     * The following functions are intended to be invoked from QML in a static way.
     */
    Q_INVOKABLE static bool startLokinetProcess();
    Q_INVOKABLE static bool stopLokinetProcess();
    Q_INVOKABLE static bool forciblyStopLokinetProcess();
    Q_INVOKABLE static bool managedStopLokinetProcess();
    Q_INVOKABLE static bool stopLokinetIfWeStartedIt();
    Q_INVOKABLE static bool isLokinetRunning();

    Q_INVOKABLE static void downloadBootstrapFile(const QJSValue& callback);

    Q_INVOKABLE static QPoint getAbsoluteCursorPosition();
};
 
#endif // __LOKI_PLATFORM_DETAILS_HPP__
