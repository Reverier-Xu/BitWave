import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Window
import RxUI
import "./components"
import "./views"

FramelessWindow {
    id: window

    property bool sideBarExpanded: true

    height: 700
    minimumHeight: 700
    minimumWidth: 1200
    objectName: "mainWindow"
    visible: true
    width: 1200

    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }

    Rectangle {
        id: centralWidget

        anchors.fill: parent
        anchors.margins: window.visibility === Window.Windowed ? 10 : 0
        border.color: Color.transparent(Style.palette.windowText, 0.08)
        border.width: window.visibility === Window.Windowed ? 1 : 0
        color: Style.palette.window

        SideBar {
            id: sideBar

            anchors.bottom: parent.bottom
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.left: parent.left
            anchors.leftMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.top: parent.top
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            expanded: window.sideBarExpanded
            objectName: "sideBar"
        }

        StackLayout {
            id: stack

            anchors.left: sideBar.right
            anchors.top: parent.top
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.right: parent.right
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0

            currentIndex: {
                let rootRoute = router.currentRoute.split("/")[0];
                switch (rootRoute) {
                    case "player":
                        return 0;
                    case "search":
                        return 1;
                    case "explore":
                        return 2;
                    case "libraries":
                        return 3;
                    case "playlists":
                        return 4;
                    case "settings":
                        return 5;
                    default:
                        return 0;
                }
            }

            PlayerView {
                id: playerView
            }

            SearchView {
                id: searchView
            }

            ExploreView {
                id: exploreView
            }

            LibraryView {
                id: libraryView
            }

            PlaylistView {
                id: playlistView
            }

            SettingsView {
                id: settingsView
            }
        }

        TitleBar {
            id: titleBar

            anchors.left: sideBar.right
            anchors.right: parent.right
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.top: parent.top
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            objectName: "titleBar"
        }
        ControlBar {
            id: controlBar

            anchors.bottom: parent.bottom
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.left: sideBar.right
            anchors.right: parent.right
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0
            objectName: "controlBar"
        }
    }

    Connections {
        target: ui

        function onRaiseWindowRequested() {
            window.show();
            window.raise();
            window.requestActivate();
        }
    }
}
