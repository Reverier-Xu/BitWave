set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt_add_library(${PROJECT_NAME} STATIC assets.qrc)
set(
    QML_FILES
    Button.qml
    Frame.qml
    Label.qml
    Menu.qml
    MenuItem.qml
    Pane.qml
    Popup.qml
    InteractiveProgressBar.qml
    ScrollBar.qml
    ScrollIndicator.qml
    ScrollView.qml
    Slider.qml
    StackView.qml
    Style.qml
    TextBox.qml
    ToolTip.qml
)
set_source_files_properties(
    Style.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)
qt_add_qml_module(
    ${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qml/${PROJECT_NAME}"
    IMPORTS QtQuick.Controls.Basic
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    AUTOMOC ON
    AUTORCC ON
)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)