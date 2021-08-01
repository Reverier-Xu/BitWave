import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "../components"

Rectangle {
    id: root
    color: settings.colorStyle? "#a0ffffff":"#d0000000"
    height: 32

    signal foldSideBarTriggered()

    IconButton {
        id: closeButton
        icon: "qrc:/assets/close.svg"
        iconSize: 16
        pressedColor: "#ccee0000"
        hoverColor: "#aaff0000"
        border.color: "transparent"
        flat: true
        anchors.right: parent.right
        anchors.top: parent.top
        width: 54
        height: 32
        onClicked: {
            window.close();
        }
    }

    IconButton {
        id: maximizeButton
        icon: window.visibility === Window.Windowed?"qrc:/assets/maximize.svg":"qrc:/assets/restore.svg"
        iconSize: 16
        flat: true
        anchors.right: closeButton.left
        anchors.top: parent.top
        border.color: "transparent"
        width: 54
        height: 32
        onClicked: {
            if (window.visibility === Window.Maximized || window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showMaximized();
        }
    }

    IconButton {
        id: fullscreenButton
        width: 54
        height: 32
        iconSize: 16
        flat: true
        anchors.right: maximizeButton.left
        anchors.top: parent.top
        border.color: "transparent"
        icon: window.visibility === Window.FullScreen ? "qrc:/assets/exit-fullscreen.svg" : "qrc:/assets/enter-fullscreen.svg"
        onClicked: {
            if (window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showFullScreen();
        }
    }

    IconButton {
        id: minimizeButton
        icon: "qrc:/assets/minimize.svg"
        iconSize: 16
        flat: true
        anchors.right: fullscreenButton.left
        anchors.top: parent.top
        border.color: "transparent"
        width: 54
        height: 32
        onClicked: {
            window.showMinimized();
        }
    }

    IconButton {
        id: colorStyleButton
        icon: settings.colorStyle? "qrc:/assets/sun.svg":"qrc:/assets/moon.svg"
        iconSize: 16
        flat: true
        anchors.right: minimizeButton.left
        anchors.top: parent.top
        border.color: "transparent"
        width: 54
        height: 32
        onClicked: {
            settings.colorStyle = !settings.colorStyle
        }
    }

    PushButton {
        id: titleButton
        icon: "qrc:/assets/navigation.svg"
        text: qsTr("Playing Now")
        anchors.left: parent.left
        anchors.top: parent.top
        height: 32
        flat: true
        border.color: "transparent"
        onClicked: {
            root.foldSideBarTriggered();
        }
    }

    TapHandler {
        onTapped: if (tapCount === 2) toggleMaximized();
        gesturePolicy: TapHandler.DragThreshold
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
