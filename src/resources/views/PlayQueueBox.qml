import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: "transparent"

    TextLabel {
        id: queueTitle
        text: qsTr("Media Queue")
        icon: "qrc:/assets/list.svg"
        height: 32
        anchors.top: parent.top
        anchors.left: parent.left
    }

    ListView {
        id: queue
        anchors.top: queueTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
