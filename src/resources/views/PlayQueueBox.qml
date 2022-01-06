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

        highlightRangeMode: ListView.StrictlyEnforceRange
        currentIndex: queue.queuePos

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
        delegate: QueueMediaItem {
            width: ListView.view.width
            theId: index
            theTitle: mediaTitle
            theDuration: mediaDuration
            theStatus: index === queue.queuePos
            onClicked: {
                queue.userSwitchRequested(index);
            }
            onPlayTriggered: {
                queue.userSwitchRequested(triggerId);
            }
            onRemoveTriggered: {
                queue.removeMedia(triggerId);
            }
            onClearTriggered: {
                queue.clearQueue();
            }
        }
    }
}
