cmake_minimum_required(VERSION 3.12)

project(BitWave LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${ECM_MODULE_PATH})

include(cmake/ParseArguments.cmake)
include(cmake/OptionalSource.cmake)
include(cmake/Summary.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_MIN_VERSION 5.6.0)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Widgets Network Sql OpenGL Quick)
set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Network Qt5::Sql Qt5::OpenGL Qt5::Quick)

if(APPLE)
    find_package(Qt5 REQUIRED COMPONENTS MacExtras)
    list(APPEND QT_LIBRARIES Qt5::MacExtras)
elseif(WIN32)
    find_package(Qt5 REQUIRED COMPONENTS WinExtras)
    list(APPEND QT_LIBRARIES Qt5::WinExtras)
else()
    find_package(Qt5 REQUIRED COMPONENTS DBus)
    list(APPEND QT_LIBRARIES Qt5::DBus)
    get_target_property(QT_DBUSXML2CPP_EXECUTABLE Qt5::qdbusxml2cpp LOCATION)
endif()

find_package(MPV REQUIRED)

find_package(FFmpeg REQUIRED)

set(FFMPEG_LIBRARIES
        FFmpeg::avutil
        FFmpeg::avformat
        FFmpeg::avfilter)

add_subdirectory(3rd/QtAES)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

add_subdirectory(3rd/singleapplication)

add_subdirectory(src)
