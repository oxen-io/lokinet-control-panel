TEMPLATE = app

QT += qml quick widgets

SOURCES += src/main.cpp \
           src/QmlClipboardAdapter.cpp \
           src/HttpClient.cpp \
           src/LokinetApiClient.cpp \
           src/StatFetcher.cpp \
           src/PlatformDetails.cpp \

HEADERS += src/QmlClipboardAdapter.hpp \
           src/HttpClient.hpp \
           src/LokinetApiClient.hpp \
           src/StatFetcher.hpp \
           src/PlatformDetails.hpp \

RESOURCES += res/resources.qrc

