TEMPLATE = app

TARGET = lokinet-gui

QT += qml quick widgets charts

CONFIG += c++14

SOURCES += src/main.cpp \
           src/QmlClipboardAdapter.cpp \
           src/HttpClient.cpp \
           src/LokinetApiClient.cpp \
           src/ApiPoller.cpp \
           src/PlatformDetails.cpp \
           src/BandwidthChartData.cpp \
           src/process/LokinetProcessManager.cpp \

HEADERS += src/QmlClipboardAdapter.hpp \
           src/HttpClient.hpp \
           src/LokinetApiClient.hpp \
           src/ApiPoller.hpp \
           src/PlatformDetails.hpp \
           src/BandwidthChartData.hpp \
           src/process/LokinetProcessManager.hpp \

RESOURCES += res/resources.qrc

macx {
    LIBS += -framework Security -framework CoreFoundation
    HEADERS += src/process/MacOSLokinetProcessManager.hpp
    SOURCES += src/process/MacOSLokinetProcessManager.cpp
}
unix:!macx {
    'invoke this via: qmake SYSTEMD=true ..'
    defined(SYSTEMD, var) {
        message("systemd");
        DEFINES+=SYSTEMD
        HEADERS += src/process/SystemdLokinetProcessManager.hpp
        SOURCES += src/process/SystemdLokinetProcessManager.cpp
    } else {
        message("linux");
        HEADERS += src/process/LinuxLokinetProcessManager.hpp
        SOURCES += src/process/LinuxLokinetProcessManager.cpp
    }
}
win32 {
    HEADERS += src/process/WindowsLokinetProcessManager.hpp
    SOURCES += src/process/WindowsLokinetProcessManager.cpp
}
