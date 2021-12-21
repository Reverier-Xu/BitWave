cmake_minimum_required(VERSION 3.12)

project(BitWave LANGUAGES C CXX)

set(CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH})

include(cmake/ParseArguments.cmake)
include(cmake/OptionalSource.cmake)
include(cmake/Summary.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION 5.6.0)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Widgets Network Sql OpenGL Quick)
if(HAVE_X11)
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS X11Extras)
    find_package(Qt5DBus ${QT_MIN_VERSION})
endif()
if(APPLE)
    find_package(Qt5 REQUIRED COMPONENTS MacExtras)
endif()
if(WIN32)
    find_package(Qt5 REQUIRED COMPONENTS WinExtras)
endif()

set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Network Qt5::Sql Qt5::OpenGL Qt5::Quick)

if(Qt5DBus_FOUND)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::DBus)
    get_target_property(QT_DBUSXML2CPP_EXECUTABLE Qt5::qdbusxml2cpp LOCATION)
endif()
if(HAVE_X11)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::X11Extras)
endif()
if(WIN32)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::WinExtras)
endif()

find_package(MPV REQUIRED)

find_package(FFmpeg REQUIRED)

set(FFMPEG_LIBRARIES
        FFmpeg::avutil
        FFmpeg::avformat
        FFmpeg::avfilter)

add_subdirectory(3rd/QtAES)

if (NOT WIN32)
    find_package(Qt5DBus REQUIRED)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::DBus)
    add_definitions(-DQt5DBus_FOUND)
endif ()

add_definitions(${QT_DEFINITIONS})

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

add_subdirectory(3rd/singleapplication)

add_subdirectory(src)
