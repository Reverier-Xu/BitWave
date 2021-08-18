import QtQuick 2.15

PushArea {
    id: root
    height: 36
    flat: true
    property double downloadProgress: 0
    property bool downloaded: false
    property int id: 0
    property string title: "No title"
    property string artist: "No artist"
    property string album: "No album"
    property double duration: 0
    property bool isLocal: true

    TextLabel {
        id: idLabel
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        text: root.id.toString().padStart(4, "0")
        color: "#80808080"
    }

    IconButton {
        id: addToPlaylistButton
        anchors.left: idLabel.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: height
        iconSize: 16
        icon: "qrc:/assets/add.svg"
    }

    TextLabel {
        id: titleLabel
        anchors.left: addToPlaylistButton.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    TextLabel {
        id: timeLabel
        anchors.right: parent.right
    }
}
