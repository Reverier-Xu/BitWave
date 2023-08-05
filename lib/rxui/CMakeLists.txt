cmake_minimum_required(VERSION 3.20)

project(
    RxUI
    VERSION 0.1.0
    DESCRIPTION "A flat design component library for QML."
    LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Gui QuickControls2)

qt_standard_project_setup()
set(QT_LIBRARIES Qt6::Core Qt6::Quick Qt6::Gui Qt6::QuickControls2)

add_subdirectory(src)
