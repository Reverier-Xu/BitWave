cmake_minimum_required(VERSION 3.12)

project(BitWave LANGUAGES C CXX)

set(CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui Sql OpenGL Network Quick Multimedia)
set(QT_LIBRARIES  Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql Qt5::OpenGL Qt5::Network Qt5::Quick Qt5::Multimedia)
find_package(MPV REQUIRED)

add_subdirectory(3rd/QtAES)

if (WIN32)
    find_package(ZLIB REQUIRED)
endif (WIN32)

if (NOT WIN32)
    find_package(Qt5DBus)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::DBus)
    add_definitions(-DQt5DBus_FOUND)
endif()

add_definitions(${QT_DEFINITIONS})

add_subdirectory(src)
