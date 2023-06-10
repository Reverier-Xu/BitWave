cmake_minimum_required(VERSION 3.20)

project(
    BitWave
    DESCRIPTION "Simple media-player."
    VERSION 2.0.0
    LANGUAGES C CXX
)


SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Gui QuickControls2 Widgets Network Sql OpenGL Concurrent)
qt_standard_project_setup()
set(QT_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Network Qt6::Sql Qt6::OpenGL Qt6::Quick Qt6::Gui Qt6::QuickControls2 Qt6::Concurrent)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

if(APPLE)
    find_package(Qt6 REQUIRED COMPONENTS MacExtras)
    list(APPEND QT_LIBRARIES Qt6::MacExtras)
elseif(WIN32)
    find_package(Qt6 REQUIRED COMPONENTS WinExtras)
    list(APPEND QT_LIBRARIES Qt6::WinExtras)
else()
    find_package(Qt6 REQUIRED COMPONENTS DBus)
    list(APPEND QT_LIBRARIES Qt6::DBus)
    get_target_property(QT_DBUSXML2CPP_EXECUTABLE Qt6::qdbusxml2cpp LOCATION)
endif()

find_package(MPV REQUIRED)
find_package(FFmpeg REQUIRED)

add_subdirectory(lib/aes)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
add_subdirectory(lib/singleapplication)

add_subdirectory(lib/rxui)

add_subdirectory(src)
