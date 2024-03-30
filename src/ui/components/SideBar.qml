import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: sideBar

    property bool expanded: true

    color: Color.transparent(Style.palette.windowText, 0.08)
    width: expanded ? 280 : 0

    ActiveTab {
        id: logoTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        font.bold: true
        height: 48
        icon.color: "transparent"
        icon.height: 28
        icon.source: "qrc:/assets/logo-fill.svg"
        icon.width: 28
        hoverColor: "transparent"
        pressedColor: "transparent"
        text: "Bit Wave!"
    }

    ActiveTab {
        id: searchTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: logoTab.bottom
        anchors.topMargin: 6
        hoverColor: "transparent"
        pressedColor: "transparent"
        selected: router.currentRoute.startsWith("search")

        TextBox {
            id: searchBox

            property string lastSearchText: ""

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            placeholder: lastSearchText ? lastSearchText : qsTr("Hi, how are you?")
            onTabPressed: {
                inputText = lastSearchText;
            }
            onInputFinished: {
                lastSearchText = inputText;
                router.push(`search/${inputText}`);
                inputText = "";
            }
        }

    }

    ActiveTab {
        id: playerTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchTab.bottom
        anchors.topMargin: 12
        icon.source: "qrc:/qt/qml/RxUI/assets/play.svg"
        text: qsTr("Playing now")
        selected: router.currentRoute.startsWith("player")
        onClicked: {
            router.push("player");
        }
    }

    ActiveTab {
        id: libraryTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playerTab.bottom
        anchors.topMargin: 4
        icon.source: "qrc:/qt/qml/RxUI/assets/archive.svg"
        text: qsTr("Library")
        selected: router.currentRoute.startsWith("libraries/")
        onClicked: {
            router.push("libraries/");
        }
    }

    ActiveTab {
        id: playlistTitle

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: libraryTab.bottom
        anchors.topMargin: 4
        text: qsTr("Playlists")
        textColor: Color.transparent(Style.palette.windowText, 0.6)

        icon.source: "qrc:/qt/qml/RxUI/assets/star-line-horizontal-3.svg"
        hoverColor: "transparent"
        pressedColor: "transparent"

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: Color.transparent(Style.palette.windowText, 0.08)
        }

        Button {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            icon.source: "qrc:/qt/qml/RxUI/assets/add.svg"
            display: AbstractButton.IconOnly
            flat: true
            onClicked: {
                createPlaylistPopup.open();
            }

            Popup {
                id: createPlaylistPopup

                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                padding: 8
                height: 52
                x: (parent.width - width) / 2
                y: parent.height

                Row {
                    spacing: 8

                    TextBox {
                        id: playlistName
                        height: 36

                        width: 200
                        placeholder: qsTr("Playlist name")
                    }

                    Button {
                        text: qsTr("Create")
                        height: 36
                        onClicked: {
                            if (playlistName.inputText) {
                                playlist.addPlaylist(playlistName.inputText);
                                createPlaylistPopup.close();
                            }
                        }
                    }

                }

                enter: Transition {
                    NumberAnimation {
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: 120
                    }

                    NumberAnimation {
                        property: "height"
                        from: 28
                        to: 56
                        duration: 300
                        easing.type: Easing.OutExpo
                    }

                }

                exit: Transition {
                    NumberAnimation {
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: 120
                    }

                    NumberAnimation {
                        property: "height"
                        from: 56
                        to: 28
                        duration: 300
                        easing.type: Easing.OutExpo
                    }

                }

            }

        }

    }

    ListView {
        anchors.bottom: settingTab.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playlistTitle.bottom
        anchors.topMargin: 4
        spacing: 4
        clip: true
        model: playlist

        ScrollBar.vertical: ScrollBar {
        }

        delegate: ActiveTab {
            icon.source: "qrc:/qt/qml/RxUI/assets/star-line-horizontal-3.svg"
            text: name
            width: ListView.view.width
            onClicked: {
                router.push("playlists/" + name);
            }
            selected: router.currentRoute === ("playlists/" + name)

            HoverHandler {
                id: hoverHandler
            }

            Button {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                icon.source: "qrc:/qt/qml/RxUI/assets/delete.svg"
                display: AbstractButton.IconOnly
                flat: true
                opacity: hoverHandler.hovered ? 1 : 0
                onClicked: {
                    playlist.removePlaylist(name);
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.OutExpo
                    }

                }

            }

        }

    }

    ActiveTab {
        id: settingTab

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        icon.source: "qrc:/qt/qml/RxUI/assets/settings.svg"
        text: qsTr("Settings")
        selected: router.currentRoute.startsWith("settings")
        onClicked: {
            router.push("settings");
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }

    }

}
