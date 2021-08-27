import QtQuick 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "#e0e0e0" : "#151515"
    width: 360
    clip: true
    property bool expanded: display.queueBarExpanded
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
            }
        },
        State {
            name: "Folded"
            PropertyChanges {
                target: root
                width: 0
            }
        }
    ]
}
