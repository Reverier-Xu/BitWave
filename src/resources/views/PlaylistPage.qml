import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "#1b1b1b"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    TextLabel {
        id: playlistPageTitle
        text: service.pageTitle
        fontSize: 24
        iconSize: 24
        showIcon: true
        icon: service.pageIcon
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 25
        height: 48
    }

    PushButton {
        id: refreshButton
        text: qsTr("Refresh")
        showIcon: true
        icon: "qrc:/assets/async.svg"
        anchors.verticalCenter: playlistPageTitle.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 25
        height: 36
        flat: true
        onClicked: {
            service.refresh();
            pageLoadingCover.opacity = 1;
            pageLoader.running = true;
            refreshButton.enabled = false;
        }
    }

    Rectangle {
        id: pageLoadingCover
        anchors.fill: pageContentList
        opacity: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
        
        color: display.colorStyle ? "#101b1b1b" : "#10FFFFFF"
        Loader {
            id: pageLoader
            radius: parent.width / 10
            anchors.centerIn: parent
            running: false
        }

        Connections {
            target: service
            function onContentChanged() {
                pageLoader.running = false;
                pageLoadingCover.opacity = 0;
                refreshButton.enabled = true;
            }
        }
    }

    ListView {
        id: pageContentList
        clip: true
        anchors.top: playlistPageTitle.bottom
        anchors.topMargin: 25
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        width: parent.width - 120
        model: playlistModel
        ScrollBar.vertical: UXScrollBar { }

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
        delegate: MediaTableItem {
            width: ListView.view.width
            theId: index
            theTitle: title
            theAlbum: album
            theArtist: artist
            theDuration: duration
            theUri: uri
            isEndpoint: service.currentUriIsEndpoint
            onDoubleClicked: {
                if (service.currentUriIsEndpoint) {
                    service.play(index);
                } else {
                    service.enter(uri);
                }
            }
        }
    }
}
