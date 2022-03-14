import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "qrc:/components"

FluentWindow {
    id: window
    objectName: "mainWindow"
    width: 1200
    minimumWidth: 1200
    height: 700
    minimumHeight: 700

    SystemTray {}

    KeyTapEvent {
        id: spaceEvent
        customKey: "Space"
        onClicked: {
            if (player.isMediaLoaded && player.isPlaying)
                player.pause()
            else
                player.resume()
        }
        onDoubleClicked: {
            if (window.visibility === Window.FullScreen) {
                window.showNormal();
            } else {
                window.showFullScreen();
                display.sideBarExpanded = false;
            }
        }
    }

    KeyTapEvent {
        id: exitFullScreenEvent
        customKey: "Escape"
        onClicked: {
            if (window.visibility === Window.FullScreen) {
                window.showNormal();
            }
        }
    }

    KeyTapEvent {
        id: exitAppEvent
        customKey: "Ctrl+Q"
        onClicked: {
            Qt.exit(0);
        }
    }

    KeyTapEvent {
        id: increaseTimeEvent
        customKey: "Right"
        onClicked: {
            player.handleUserSeekRequest(player.currentTime + 3)
        }
    }

    KeyTapEvent {
        id: decreaseTimeEvent
        customKey: "Left"
        onClicked: {
            player.handleUserSeekRequest(player.currentTime - 3)
        }
    }

    KeyTapEvent {
        id: increaseVolumeEvent
        customKey: "Up"
        onClicked: {
            player.volume += 0.1;
        }
    }

    KeyTapEvent {
        id: decreaseVolumeEvent
        customKey: "Down"
        onClicked: {
            player.volume -= 0.1;
        }
    }

    Rectangle {
        id: centralWidget
        anchors.fill: parent
        anchors.margins: window.visibility === Window.Windowed ? 10 : 0
        border.width: window.visibility === Window.Windowed ? 1 : 0
        border.color: "#40606060"
        color: display.colorStyle? "#e0e0e0" : "#242424"

        Behavior on color {
            ColorAnimation {
                duration: 280
            }
        }

        SideBar {
            id: sideBar
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
        }

        PageStack {
            id: pageStack
            anchors.left: sideBar.right
            anchors.top: titleBar.top
            anchors.bottom: controlBar.bottom
            anchors.right: titleBar.right
        }

        TitleBar {
            id: titleBar
            anchors.top: parent.top
            anchors.left: sideBar.right
            anchors.right: parent.right
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0
        }

        ControlBar {
            id: controlBar
            anchors.left: sideBar.right
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0
        }

        QueueBar {
            id: queueBar
            anchors.right: titleBar.right
            anchors.top: titleBar.bottom
            anchors.bottom: controlBar.top
        }
    }

    Connections {
        target: display
        function onRaiseWindowRequested() {
            window.show();
            window.raise();
            window.requestActivate();
        }
    }
}
