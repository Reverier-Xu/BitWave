import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RxUI

Rectangle {
    id: control
    color: Color.transparent(Style.palette.window, 0.95)

    Item {
        id: title
        height: 36
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 32
        anchors.right: parent.right
        anchors.rightMargin: 32

        Label {
            anchors.left: parent.left
            text: qsTr("Play Queue")
            anchors.verticalCenter: parent.verticalCenter
        }

        RowLayout {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 8

            Label {
                text: qsTr("Mode: ")
            }

            Button {
                id: orderModeButton

                display: AbstractButton.IconOnly
                height: 36
                icon.source: "qrc:/qt/qml/RxUI/assets/play-order.svg"
                radius: width / 2
                width: 36
                flat: queue.mode !== 0
                onClicked: {
                    queue.mode = 0
                }
            }

            Button {
                id: reverseModeButton

                display: AbstractButton.IconOnly
                height: 36
                icon.source: "qrc:/qt/qml/RxUI/assets/play-reverse.svg"
                radius: width / 2
                width: 36
                flat: queue.mode !== 1
                onClicked: {
                    queue.mode = 1
                }
            }

            Rectangle {
                width: 1
                color: Style.palette.mid
                height: 16
            }

            Button {
                id: repeatAllModeButton

                display: AbstractButton.IconOnly
                height: 36
                icon.source: "qrc:/qt/qml/RxUI/assets/play-repeat-all.svg"
                radius: width / 2
                width: 36
                flat: queue.mode !== 2
                onClicked: {
                    queue.mode = 2
                }
            }

            Button {
                id: repeatOneModeButton

                display: AbstractButton.IconOnly
                height: 36
                icon.source: "qrc:/qt/qml/RxUI/assets/play-repeat-one.svg"
                radius: width / 2
                width: 36
                flat: queue.mode !== 3
                onClicked: {
                    queue.mode = 3
                }
            }

            Rectangle {
                width: 1
                color: Style.palette.mid
                height: 16
            }

            Button {
                id: randomModeButton

                display: AbstractButton.IconOnly
                height: 36
                icon.source: "qrc:/qt/qml/RxUI/assets/play-random.svg"
                radius: width / 2
                width: 36
                flat: queue.mode !== 4
                onClicked: {
                    queue.mode = 4
                }
            }
        }
    }

    ListView {
        id: queueList
        clip: true
        anchors.top: title.bottom
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 32
        anchors.right: parent.right
        anchors.rightMargin: 32
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16

        ScrollBar.vertical: ScrollBar {}
        model: queueModel

        highlightRangeMode: ListView.ApplyRange
        highlightMoveDuration: 300
        highlightMoveVelocity: -1
        currentIndex: queue.cursor

        add: Transition {
            NumberAnimation { properties: "x"; from: 100; duration: 300; easing.type: Easing.OutExpo; }
            NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 200; }
        }
        addDisplaced: Transition {
            NumberAnimation { properties: "y"; duration: 200; easing.type: Easing.OutExpo; }
        }
        removeDisplaced: Transition {
            PauseAnimation { duration: 300; }
            NumberAnimation { properties: "y"; duration: 200 }
        }
        remove: Transition {
            NumberAnimation { properties: "x"; to: 100; duration: 300; easing.type: Easing.OutExpo; }
            NumberAnimation { properties: "opacity"; from: 1; to: 0; duration: 300; easing.type: Easing.OutExpo; }
        }

        delegate: MediaRow {
            width: ListView.view.width
            index: mediaId
            title: mediaTitle
            artists: mediaArtists.join(', ')
            album: mediaAlbum
            time: mediaTime
            playing: queue.cursor === mediaId

            onClicked: {
                queue.play(mediaId)
            }
        }
    }

    Rectangle {
        id: queueLoadingCover
        color: Color.transparent(Style.palette.window, 0.8)
        anchors.fill: queueList
        visible: queue.loading

        Loader {
            anchors.centerIn: parent
            radius: 32

            running: queue.loading
        }
    }
}
