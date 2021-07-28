cmake_minimum_required(VERSION 3.12)

project(BitWave LANGUAGES C CXX)

set(CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui Sql OpenGL Network Quick)
set(QT_LIBRARIES  Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql Qt5::OpenGL Qt5::Network Qt5::Quick)
find_package(MPV REQUIRED)

message(STATUS "Using Embed Taglib library")
set(TAGLIB_VERSION 1.11.1)
set(TAGLIB_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/3rd/taglib/headers/taglib/;${CMAKE_BINARY_DIR}/3rd/taglib/headers/")
set(TAGLIB_LIBRARY_DIRS "")
set(TAGLIB_LIBRARIES tag)
set(TAGLIB_HAS_OPUS ON)
add_subdirectory(3rd/taglib)
add_definitions(-DTAGLIB_STATIC)
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
