import QtQuick 2.15
import QtQuick.Controls 2.15
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
        id: queueList
        clip: true
        anchors.top: queueTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: mainQueueModel
        ScrollBar.vertical: UXScrollBar { }
        delegate: QueueMediaItem {
            width: ListView.view.width
            theId: mediaId
            theTitle: mediaTitle
            theDuration: mediaDuration
            theStatus: mediaId === queue.queuePos
            onClicked: {
                queue.queuePos = mediaId;
            }
        }
    }
}
