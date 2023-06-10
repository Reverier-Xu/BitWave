import QtQuick
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

    Behavior on color  {
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

        StackView {
            id: stack

            anchors.left: sideBar.right
            anchors.top: parent.top
            anchors.topMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: window.visibility === Window.Windowed ? 1 : 0
            anchors.right: parent.right
            anchors.rightMargin: window.visibility === Window.Windowed ? 1 : 0

            initialItem: playerView

            currentItem: {
                let rootRoute = router.currentRoute.split("/")[0];
                switch (rootRoute) {
                    case "explore":
                        return exploreView;
                    case "libraries":
                        return libraryView;
                    case "settings":
                        return settingsView;
                    case "playlists":
                        return playlistView;
                    case "search":
                        return searchView;
                    default:
                        return playerView;
                }
            }

            PlayerView {
                id: playerView
            }

            ExploreView {
                id: exploreView
            }

            LibraryView {
                id: libraryView
            }

            SettingsView {
                id: settingsView
            }

            PlaylistView {
                id: playlistView
            }

            SearchView {
                id: searchView
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
