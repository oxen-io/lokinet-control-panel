cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)  # Has to be set before `project()`, and ignored on non-macos

project(lokinet-gui
    VERSION 0.2.2
    DESCRIPTION "Lokinet graphical control panel"
    HOMEPAGE_URL https://github.com/loki-project/loki-network-control-panel
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
  
find_package(Qt5 COMPONENTS Qml Quick Widgets Charts REQUIRED)
find_package(Threads)
find_package(LokiMQ 1.2 REQUIRED)

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
    lokimq
    )

set_target_properties(lokinet-gui
    PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON

        AUTORCC ON  # Enables processing *.qrc in sources
        AUTOMOC ON  # Does... whatever automoc is
    )

if(APPLE)
    option(MACOS_APP "Build a LokinetGUI.app bundle instead of a binary" OFF)
    set(MACOS_SIGN ""
        CACHE STRING "enable codesigning -- use a 'Apple Distribution' key (or key description) from `security find-identity -v`")

    target_sources(lokinet-gui PRIVATE src/process/MacOSLokinetProcessManager.cpp)

    if(MACOS_APP)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lokinet-gui.icns
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/mk-icns.sh ${CMAKE_CURRENT_SOURCE_DIR}/res/images/icon.svg ${CMAKE_CURRENT_BINARY_DIR}/lokinet-gui.icns
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/images/icon.svg ${CMAKE_CURRENT_SOURCE_DIR}/tools/mk-icns.sh)

        target_sources(lokinet-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/lokinet-gui.icns)

        set_target_properties(lokinet-gui
            PROPERTIES
            MACOSX_BUNDLE TRUE
            OUTPUT_NAME LokinetGUI
            RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/lokinet-gui.icns")

        set(MACOSX_BUNDLE_BUNDLE_NAME LokinetGUI)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER org.lokinet.lokinet-gui)
        set(MACOSX_BUNDLE_INFO_STRING "Lokinet control panel")
        set(MACOSX_BUNDLE_ICON_FILE lokinet-gui.icns)
        set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
        set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
        set(MACOSX_BUNDLE_COPYRIGHT "© 2020, The Loki Project")

        get_target_property(uic_location Qt5::uic IMPORTED_LOCATION)
        get_filename_component(qt_dir ${uic_location} DIRECTORY)
        if(MACOS_SIGN)
            add_custom_command(TARGET lokinet-gui
                POST_BUILD
                COMMAND echo "Running qt magic macos deploy script"
                COMMAND "${qt_dir}/macdeployqt" LokinetGUI.app -always-overwrite -qmldir=${PROJECT_SOURCE_DIR}/res
                COMMAND echo "Signing LokinetGUI.app and everything inside it"
                COMMAND codesign -s "${MACOS_SIGN}" --deep --strict --options runtime
                    --entitlements ${PROJECT_SOURCE_DIR}/tools/lokinet-gui-entitlements.plist --force -vvv LokinetGUI.app
                )
        else()
            add_custom_command(TARGET lokinet-gui
                POST_BUILD
                COMMAND echo "Running qt magic macos deploy script"
                COMMAND "${qt_dir}/macdeployqt" LokinetGUI.app -always-overwrite
                )
        endif()
    endif()
elseif(WIN32)
    target_sources(lokinet-gui PRIVATE src/process/WindowsLokinetProcessManager.cpp)
else()
    option(SYSTEMD "Enable systemd support" OFF)
    if(SYSTEMD)
        target_sources(lokinet-gui PRIVATE src/process/SystemdLokinetProcessManager.cpp)
        target_compile_definitions(lokinet-gui PRIVATE SYSTEMD)
    else()
        target_sources(lokinet-gui PRIVATE src/process/LinuxLokinetProcessManager.cpp)
    endif()
endif()

install(TARGETS lokinet-gui
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
    RESOURCE DESTINATION .)
