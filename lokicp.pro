TEMPLATE = app

QT += qml quick widgets

SOURCES += src/main.cpp \
           src/QmlClipboardAdapter.cpp \
           src/HttpClient.cpp \
           src/StatFetcher.cpp \

HEADERS += src/QmlClipboardAdapter.hpp \
           src/HttpClient.hpp \
           src/StatFetcher.hpp \

RESOURCES += res/resources.qrc

