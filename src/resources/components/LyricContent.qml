import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    property bool isActive: false
    property string lyricText: ""
    height: lyricTextItem.contentHeight + 12
    color: "transparent"

    TextEdit {
        id: lyricTextItem
        readOnly: true
        text: root.lyricText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
        font.pixelSize: 16
        // font.bold: root.isActive
        color: root.isActive ? (settings.colorStyle ? "black" : "white") : "#808080"
    }

    PushArea {
        id: hoverArea
        flat: true
        anchors.fill: parent
        border.color: "transparent"
    }
}
