import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RxUI

Rectangle {
    id: view
    color: Style.palette.window

    Item {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.leftMargin: 32
        anchors.rightMargin: 32
        anchors.topMargin: 48
        height: 36

        Label {
            id: title

            font.bold: true
            height: 36
            text: qsTr("Library")
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        Button {
            id: musicFilterButton
            height: 36
            icon.height: 16
            icon.width: 16
            text: qsTr("Music")
            flat: true
            anchors.right: videoFilterButton.left
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            display: AbstractButton.TextBesideIcon
            icon.source: "qrc:/qt/qml/RxUI/assets/music-note-2.svg"

            onClicked: {
                router.push(`libraries/music`);
            }
        }

        Button {
            id: videoFilterButton
            height: 36
            text: qsTr("Video")
            flat: true
            display: AbstractButton.TextBesideIcon
            icon.height: 16
            icon.width: 16
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/qt/qml/RxUI/assets/movies-and-tv.svg"

            onClicked: {
                router.push(`libraries/video`);
            }
        }

        Rectangle {
            id: filterIndicator
            height: 3
            width: musicFilterButton.width - 12
            color: Style.primary
            anchors.top: musicFilterButton.bottom
            anchors.topMargin: -6
            anchors.horizontalCenter: library.filter === "music" ? musicFilterButton.horizontalCenter : videoFilterButton.horizontalCenter

            states: [
                State {
                    name: "music"
                    AnchorChanges {
                        target: filterIndicator; anchors.horizontalCenter: musicFilterButton.horizontalCenter
                    }
                },
                State {
                    name: "video"
                    AnchorChanges {
                        target: filterIndicator; anchors.horizontalCenter: videoFilterButton.horizontalCenter
                    }
                }
            ]

            transitions: Transition {
                AnchorAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }
        }
    }

    Connections {
        target: router

        function onCurrentRouteChanged(route) {
            // console.log("route changed", route)
            if (route.startsWith("libraries")) {
                library.load(route)
            }
        }
    }

    Connections {
        target: library
        function onFilterChanged(filter) {
            filterIndicator.state = filter
        }
    }
}
