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

    StackLayout {
        id: layout
        anchors.fill: parent
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

    transitions: [
        Transition {
            from: "*"; to: "Expanded"
            NumberAnimation {
                target: root
                property: "width"
                duration: 300
                easing.type: Easing.OutExpo
            }
        },
        Transition {
            from: "*"; to: "Folded"
            NumberAnimation {
                target: root
                property: "width"
                duration: 300
                easing.type: Easing.OutExpo
            }
        }
    ]
}
