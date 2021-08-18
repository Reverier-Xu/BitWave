import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "qrc:/components"

FluentWindow {
    id: window
    width: 1200
    minimumWidth: 1200
    height: 700
    minimumHeight: 700

    SystemTray {}

    Rectangle {
        id: centralWidget
        anchors.fill: parent
        anchors.margins: window.visibility === Window.Windowed ? 10 : 0
        border.width: window.visibility === Window.Windowed ? 1 : 0
        border.color: "#40606060"
        color: display.colorStyle? "#e0e0e0" : "#151515"

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
            onFoldSideBarTriggered: {
                sideBar.expanded = !sideBar.expanded
            }
        }

        ControlBar {
            id: controlBar
            anchors.left: sideBar.right
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0
        }
    }
}
