import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RxUI
import "../components"

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
            text: qsTr("Library")
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        Loader {
            anchors.right: scanButton.left
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            radius: 8
            visible: library.scanning

            running: library.scanning
        }

        Button {
            id: scanButton
            height: 36
            icon.height: 16
            icon.width: 16
            text: qsTr("Scan Media")
            flat: true
            anchors.right: musicFilterButton.left
            anchors.rightMargin: 24
            anchors.verticalCenter: parent.verticalCenter
            display: AbstractButton.TextBesideIcon
            icon.source: "qrc:/qt/qml/RxUI/assets/arrow-sync.svg"
            enabled: !library.scanning

            onClicked: {
                library.scan();
            }
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

            Image {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                source: library.sortStatus === 0 ? "qrc:/qt/qml/RxUI/assets/chevron-up.svg" : "qrc:/qt/qml/RxUI/assets/chevron-down.svg"
                visible: library.sortStatus === 1 || library.sortStatus === 0
            }

            onClicked: {
                library.sortByTitle();
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

            Image {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                source: library.sortStatus === 2 ? "qrc:/qt/qml/RxUI/assets/chevron-up.svg" : "qrc:/qt/qml/RxUI/assets/chevron-down.svg"
                visible: library.sortStatus === 3 || library.sortStatus === 2
            }

            onClicked: {
                library.sortByArtists();
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

            Image {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                source: library.sortStatus === 4 ? "qrc:/qt/qml/RxUI/assets/chevron-up.svg" : "qrc:/qt/qml/RxUI/assets/chevron-down.svg"
                visible: library.sortStatus === 5 || library.sortStatus === 4
            }

            onClicked: {
                library.sortByAlbum();
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

        ScrollBar.vertical: ScrollBar {}
        model: libraryModel

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
                queue.loadFromLibrary(indexOfThisDelegate)
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
