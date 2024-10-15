import "./components"
import "./views"
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts
import QtQuick.Window
import RxUI

FramelessWindow {
    id: window

    property bool sideBarExpanded: true

    height: 700
    minimumHeight: 700
    minimumWidth: 1200
    objectName: "mainWindow"
    visible: true
    width: 1200
    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
        Style.isDark = ui.colorStyle;
    }
    onVisibilityChanged: {
        ui.fullscreen = (window.visibility === Window.FullScreen);
    }

    SystemTray {}

    KeyTapEvent {
        id: spaceEvent

        customKey: "Space"
        onClicked: {
            if (player.playing)
                player.pause();
            else
                player.resume();
        }
        onDoubleClicked: {
            if (window.visibility === Window.FullScreen) {
                window.showNormal();
            } else {
                window.showFullScreen();
                ui.sideBarExpanded = false;
            }
        }
    }

    KeyTapEvent {
        id: exitFullScreenEvent

        customKey: "Escape"
        onClicked: {
            if (window.visibility === Window.FullScreen)
                window.showNormal();
        }
    }

    KeyTapEvent {
        id: nextEvent

        customKey: "End"
        onClicked: {
            queue.next();
        }
    }

    KeyTapEvent {
        id: prevEvent

        customKey: "Home"
        onClicked: {
            queue.prev();
        }
    }

    KeyTapEvent {
        id: exitAppEvent

        customKey: "Ctrl+Q"
        onClicked: {
            app.requestQuit();
        }
    }

    KeyTapEvent {
        id: increaseTimeEvent

        customKey: "Right"
        onClicked: {
            player.seek(player.currentTime + 3);
        }
    }

    KeyTapEvent {
        id: decreaseTimeEvent

        customKey: "Left"
        onClicked: {
            player.seek(player.currentTime - 3);
        }
    }

    KeyTapEvent {
        id: increaseVolumeEvent

        customKey: "Up"
        onClicked: {
            player.toggleVolume(player.volume + 10);
        }
    }

    KeyTapEvent {
        id: decreaseVolumeEvent

        customKey: "Down"
        onClicked: {
            player.toggleVolume(player.volume - 10);
        }
    }

    Rectangle {
        id: centralWidget

        anchors.fill: parent
        anchors.margins: window.visibility === Window.Windowed ? 10 : 0
        border.color: Qt.alpha(Style.palette.windowText, 0.08)
        border.width: window.visibility === Window.Windowed ? 2 : 0
        color: Style.palette.window

        Item {
            anchors.fill: parent
            anchors.margins: window.visibility === Window.Windowed ? 2 : 0

            SideBar {
                id: sideBar

                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top
                expanded: ui.sideBarExpanded
                objectName: "sideBar"
            }

            StackLayout {
                id: stack

                anchors.bottom: parent.bottom
                anchors.left: sideBar.right
                anchors.right: parent.right
                anchors.top: parent.top
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

            StackLayout {
                id: bottomStack

                anchors.bottom: sideBar.bottom
                anchors.left: sideBar.right
                anchors.right: controlBar.right
                currentIndex: controlBar.queueVisible ? 0 : 1
                height: ui.controlWidgetExpanded ? sideBar.height - titleBar.height - controlBar.height : 0

                QueueWidget {}

                OptionWidget {}

                Behavior on height {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.OutExpo
                    }
                }
            }

            TitleBar {
                id: titleBar

                anchors.left: sideBar.right
                anchors.right: parent.right
                anchors.top: sideBar.top
                objectName: "titleBar"
            }

            ControlBar {
                id: controlBar

                anchors.bottom: bottomStack.top
                anchors.left: sideBar.right
                anchors.right: parent.right
                objectName: "controlBar"
            }
        }
    }

    Connections {
        function onRaiseWindowRequested() {
            window.show();
            window.raise();
            window.requestActivate();
        }

        target: ui
    }

    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
}
