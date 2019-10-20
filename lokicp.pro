TEMPLATE = app

QT += qml quick widgets

SOURCES += src/main.cpp \
           src/QmlClipboardAdapter.cpp \
           src/HttpClient.cpp \
           src/LokinetApiClient.cpp \
           src/ApiPoller.cpp \
           src/PlatformDetails.cpp \

HEADERS += src/QmlClipboardAdapter.hpp \
           src/HttpClient.hpp \
           src/LokinetApiClient.hpp \
           src/ApiPoller.hpp \
           src/PlatformDetails.hpp \

RESOURCES += res/resources.qrc

