import QtQuick 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "#f0f0f0" : "#101010"
    width: 360
    clip: true
    property bool expanded: display.queueBarExpanded && display.mouseIsActive
    state: expanded? "Expanded" : "Folded"

    Behavior on width {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 280
            easing.type: Easing.OutExpo
        }
    }

    StackLayout {
        id: layout
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 360
        clip: true
        currentIndex: display.queueBarIndex

        PlayerToolBox { }

        PlayQueueBox { }
    }

    states: [
        State {
            name: "Expanded"
            PropertyChanges {
                target: root
                width: 360
                opacity: 1
            }
        },
        State {
            name: "Folded"
            PropertyChanges {
                target: root
                width: 240
                opacity: 0
            }
        }
    ]

    HoverHandler {
        onHoveredChanged: {
            if (hovered) {
                // console.log("Queue hovered");
                display.blockDelayedHide();
            } else {
                // console.log("Queue unhovered");
                display.delayedHide();
            }
        }
    }
}
