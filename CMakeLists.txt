cmake_minimum_required(VERSION 3.10)

project(lokinet-gui
    VERSION 0.2.2
    DESCRIPTION "Lokinet graphical control panel"
    HOMEPAGE_URL https://github.com/loki-project/loki-network-control-panel
    LANGUAGES CXX)

find_package(Qt5 COMPONENTS Qml Quick Widgets Charts REQUIRED)
find_package(Threads)

add_executable(lokinet-gui
    src/main.cpp
    src/QmlClipboardAdapter.cpp
    src/HttpClient.cpp
    src/LokinetApiClient.cpp
    src/ApiPoller.cpp
    src/PlatformDetails.cpp
    src/BandwidthChartData.cpp
    src/process/LokinetProcessManager.cpp

    res/resources.qrc
    )

target_link_libraries(lokinet-gui PRIVATE
    Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Charts
    Threads::Threads
    )

set_target_properties(lokinet-gui
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON

        AUTORCC ON  # Enables processing *.qrc in sources
        AUTOMOC ON  # Does... whatever automoc is
    )

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    option(SYSTEMD "Enable systemd support" OFF)
    if(SYSTEMD)
        target_sources(lokinet-gui PRIVATE src/process/SystemdLokinetProcessManager.cpp)
        target_compile_definitions(lokinet-gui PRIVATE SYSTEMD)
    else()
        target_sources(lokinet-gui PRIVATE src/process/LinuxLokinetProcessManager.cpp)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_sources(lokinet-gui PRIVATE src/process/MacOSLokinetProcessManager.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(lokinet-gui PRIVATE src/process/WindowsLokinetProcessManager.cpp)
else()
    message(FATAL_ERROR "Don't have support for '${CMAKE_SYSTEM_NAME}' yet!")
endif()

