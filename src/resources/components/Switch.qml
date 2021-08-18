import QtQuick 2.15

Rectangle {
    id: root
    border.width: 2
    border.color: "#80808080"
    color: "transparent"
    property bool isOn: false
    state: isOn? "Off" : "On"
    radius: height / 2
    height: 24
    width: 48

    signal clicked(var mouse)

    Rectangle {
        id: dot
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        radius: height / 2
        height: root.height - 10
        width: height
        color: "#80808080"
    }

    MouseArea {
        id: m_mousearea;
        anchors.fill: parent;
        preventStealing: true
        hoverEnabled: parent.enabled;
        acceptedButtons: Qt.LeftButton
        onClicked: {
            root.focus = true;
            root.clicked(mouse);
            root.isOn = !root.isOn;
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
            PropertyChanges {
                target: dot
                color: "#80808080"
            }
        },
        State {
            name: "On"
            AnchorChanges {
                target: dot
                anchors.left: undefined
                anchors.right: parent.right
            }
            PropertyChanges {
                target: dot
                color: display.themeColor
            }
        }
    ]

    transitions: [
        Transition {
            AnchorAnimation {
                easing.type: Easing.OutExpo
                duration: 500
            }
            ColorAnimation {
                easing.type: Easing.OutExpo
                duration: 500
            }
        }
    ]
}
