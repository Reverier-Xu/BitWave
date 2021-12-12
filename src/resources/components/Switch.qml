import QtQuick 2.15

Rectangle {
    id: root
    color: "transparent"
    property bool isOn: true
    state: isOn? "On" : "Off"
    height: 24
    width: 48

    signal clicked(var mouse)

    Rectangle {
        id: knob
        color: root.isOn ? display.themeColor : "#808080"
        opacity: 0.5
        height: 6
        radius: height / 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.verticalCenter: parent.verticalCenter

        Behavior on color {
            ColorAnimation {
                easing.type: Easing.OutExpo
                duration: 500
            }
        }
    }

    Rectangle {
        id: dot
        anchors.verticalCenter: parent.verticalCenter
        radius: height / 2
        height: root.height - 8
        width: height
        color: root.isOn ? display.themeColor : "#808080"

        Behavior on color {
            ColorAnimation {
                easing.type: Easing.OutExpo
                duration: 500
            }
        }
    }

    MouseArea {
        id: mMouseArea
        anchors.fill: parent
        preventStealing: true
        hoverEnabled: parent.enabled
        acceptedButtons: Qt.LeftButton
        onClicked: {
            root.focus = true;
            root.clicked(mouse);
        }
    }

    states: [
        State {
            name: "Off"
            AnchorChanges {
                target: dot
                anchors.left: parent.left
                anchors.right: undefined
            }
        },
        State {
            name: "On"
            AnchorChanges {
                target: dot
                anchors.left: undefined
                anchors.right: parent.right
            }
        }
    ]

    transitions: [
        Transition {
            AnchorAnimation {
                easing.type: Easing.OutExpo
                duration: 500
            }
        }
    ]
}
