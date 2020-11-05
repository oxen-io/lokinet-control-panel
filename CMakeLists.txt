cmake_minimum_required(VERSION 3.10)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)  # Has to be set before `project()`, and ignored on non-macos

project(lokinet-gui
    VERSION 0.3.0
    DESCRIPTION "Lokinet graphical control panel"
    LANGUAGES CXX)

if(WIN32)
  enable_language(RC)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

option(BUILD_STATIC_DEPS "build every dep statically (ON/OFF)" OFF)

if(BUILD_STATIC_DEPS)
  find_package(Git)
  if(GIT_FOUND)
    function(check_submodule relative_path)
      execute_process(COMMAND git rev-parse "HEAD" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${relative_path} OUTPUT_VARIABLE localHead)
      execute_process(COMMAND git rev-parse "HEAD:${relative_path}" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE checkedHead)
      string(COMPARE EQUAL "${localHead}" "${checkedHead}" upToDate)
      if (upToDate)
        message(STATUS "Submodule '${relative_path}' is up-to-date")
      else()
        message(FATAL_ERROR "Submodule '${relative_path}' is not up-to-date. Please update with\ngit submodule update --init --recursive\nor run cmake with -DSUBMODULE_CHECK=OFF")
      endif()
    endfunction ()
    message(STATUS "Checking submodules")
    check_submodule(external/loki-mq)
  endif()
  include(StaticBuild)
  add_subdirectory(external/loki-mq)
else()
  find_package(LokiMQ 1.2 REQUIRED)
endif()

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
    lokimq
    )

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-mwindows -fstack-protector")
endif()

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
    target_sources(lokinet-gui PRIVATE src/process/WindowsLokinetProcessManager.cpp src/version.rc)
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
