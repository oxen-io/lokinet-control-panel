TEMPLATE = app

QT += qml quick widgets

CONFIG += c++14

SOURCES += src/main.cpp \
           src/QmlClipboardAdapter.cpp \
           src/HttpClient.cpp \
           src/LokinetApiClient.cpp \
           src/ApiPoller.cpp \
           src/PlatformDetails.cpp \
           src/process/LokinetProcessManager.cpp \

HEADERS += src/QmlClipboardAdapter.hpp \
           src/HttpClient.hpp \
           src/LokinetApiClient.hpp \
           src/ApiPoller.hpp \
           src/PlatformDetails.hpp \
           src/process/LokinetProcessManager.hpp \

RESOURCES += res/resources.qrc

macx {
    LIBS += -framework Security -framework CoreFoundation
    HEADERS += src/process/MacOSLokinetProcessManager.hpp
    SOURCES += src/process/MacOSLokinetProcessManager.cpp
}
unix:!macx {
    HEADERS += src/process/LinuxLokinetProcessManager.hpp
    SOURCES += src/process/LinuxLokinetProcessManager.cpp
}
win32 {
    HEADERS += src/process/WindowsLokinetProcessManager.hpp
    SOURCES += src/process/WindowsLokinetProcessManager.cpp
}
