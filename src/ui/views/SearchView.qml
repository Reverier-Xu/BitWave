import "../components"
import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: view

    color: Style.palette.window

    Item {
        id: header

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
            text: qsTr("Search Results")
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row {
        id: tableHeader

        height: 36
        anchors.top: header.bottom
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 32
        anchors.rightMargin: 32

        Rectangle {
            width: 36
            height: 36
            color: "transparent"
        }

        Button {
            height: 36
            width: (parent.width - 48 - 64) / 3
            flat: true

            Label {
                anchors.fill: parent
                anchors.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHLeft
                text: qsTr("Title")
                font.bold: true
            }
        }

        Button {
            height: 36
            width: (parent.width - 48 - 64) / 3
            flat: true

            Label {
                anchors.fill: parent
                anchors.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHLeft
                text: qsTr("Artists")
                font.bold: true
            }
        }

        Button {
            height: 36
            width: (parent.width - 48 - 64) / 3
            flat: true

            Label {
                anchors.fill: parent
                anchors.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHLeft
                text: qsTr("Album")
                font.bold: true
            }
        }

        Rectangle {
            width: 72
            height: 36
            color: "transparent"

            Label {
                anchors.fill: parent
                anchors.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Duration")
                font.bold: true
            }
        }
    }

    Rectangle {
        anchors.bottom: tableHeader.bottom
        height: 2
        anchors.left: tableHeader.left
        anchors.right: tableHeader.right
        color: Style.palette.mid
    }

    ListView {
        id: mediaList

        clip: true
        anchors.top: tableHeader.bottom
        anchors.left: tableHeader.left
        anchors.right: tableHeader.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16 + 100
        model: searchModel

        ScrollBar.vertical: ScrollBar {}

        add: Transition {
            NumberAnimation {
                properties: "x"
                from: 100
                duration: 300
                easing.type: Easing.OutExpo
            }

            NumberAnimation {
                properties: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        addDisplaced: Transition {
            NumberAnimation {
                properties: "y"
                duration: 200
                easing.type: Easing.OutExpo
            }
        }

        removeDisplaced: Transition {
            PauseAnimation {
                duration: 300
            }

            NumberAnimation {
                properties: "y"
                duration: 200
            }
        }

        remove: Transition {
            NumberAnimation {
                properties: "x"
                to: 100
                duration: 300
                easing.type: Easing.OutExpo
            }

            NumberAnimation {
                properties: "opacity"
                from: 1
                to: 0
                duration: 300
                easing.type: Easing.OutExpo
            }
        }

        delegate: MediaRow {
            property int indexOfThisDelegate: index

            width: ListView.view.width
            mIndex: indexOfThisDelegate
            title: mediaTitle
            artists: mediaArtists.join(', ')
            album: mediaAlbum
            time: mediaTime
            playing: false
            canDelete: false

            onClicked: {
                queue.addMediaByUrlThenPlay(mediaUrl);
            }

            onAddToPlaylistClicked: (i, n) => {
                library.addSearchMediaIndexToPlaylist(i, n);
            }
        }
    }
    Connections {
        function onCurrentRouteChanged(route) {
            if (route.startsWith("search"))
                library.searchKeyword = route.split("/").slice(1).join('/');
        }

        target: router
    }
}
