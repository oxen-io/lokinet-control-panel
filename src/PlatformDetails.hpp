#ifndef __LOKI_PLATFORM_DETAILS_HPP__
#define __LOKI_PLATFORM_DETAILS_HPP__

#include <QObject>

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
};
 
#endif // __LOKI_PLATFORM_DETAILS_HPP__
