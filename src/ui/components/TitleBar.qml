import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: titleBar

    color: Color.transparent(Style.palette.window, 0.8)
    height: 32

    Button {
        id: closeButton

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        hoverColor: "#A0FF0000"
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/dismiss.svg"
        icon.width: 16
        pressedColor: "#E0FF0000"
        radius: 0
        width: 48

        onClicked: window.close()
    }
    Label {
        id: title

        anchors.left: backButton.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        font.bold: true
        height: parent.height / 3.5
        text: player.media.title
        verticalAlignment: Text.AlignVCenter
        opacity: 0.6
    }
    Button {
        id: maximizeButton

        anchors.right: closeButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: window.visibility === Window.Windowed ? "qrc:/qt/qml/RxUI/assets/maximize.svg" : "qrc:/qt/qml/RxUI/assets/restore.svg"
        icon.width: 16
        radius: 0
        width: 48

        onClicked: window.toggleMaximized()
    }
    Button {
        id: fullscreenButton

        anchors.right: maximizeButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: window.visibility === Window.FullScreen ? "qrc:/qt/qml/RxUI/assets/full-screen-minimize.svg" : "qrc:/qt/qml/RxUI/assets/full-screen-maximize.svg"
        icon.width: 16
        radius: 0
        width: 48

        onClicked: {
            if (window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showFullScreen();
        }
    }
    Button {
        id: minimizeButton

        anchors.right: fullscreenButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/subtract.svg"
        icon.width: 16
        radius: 0
        width: 48

        onClicked: window.showMinimized()
    }
    Button {
        id: styleButton

        anchors.right: minimizeButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: Style.isDark ? "qrc:/qt/qml/RxUI/assets/weather-moon.svg" : "qrc:/qt/qml/RxUI/assets/weather-sunny.svg"
        icon.width: 16
        radius: 0
        width: 48

        Component.onCompleted: {
            Style.isDark = displayConfig.colorStyle;
        }
        onClicked: {
            Style.isDark = !Style.isDark;
            displayConfig.colorStyle = Style.isDark;
        }
    }
    Button {
        id: foldButton

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/navigation.svg"
        icon.width: 16
        radius: 0
        width: 48

        onClicked: window.sideBarExpanded = !window.sideBarExpanded
    }
    Button {
        id: backButton

        anchors.left: foldButton.right
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 32
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/arrow-left.svg"
        icon.width: 16
        radius: 0
        width: 48

        onClicked: router.pop()
    }

    TapHandler {
        gesturePolicy: TapHandler.DragThreshold

        onTapped: if (tapCount === 2)
            window.toggleMaximized()
    }
    DragHandler {
        grabPermissions: TapHandler.DragThreshold

        onActiveChanged: {
            if (active) {
                window.startSystemMove();
            }
        }
    }
}
