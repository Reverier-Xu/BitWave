import QtQuick 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: root
    property bool flat: false
    property bool scaleOnPressed: true
    property color normalColor: flat?"transparent":"#30808080"
    property color hoverColor: flat?"#20808080":"#40808080"
    property color pressedColor: flat?"#30808080":"#60808080"
    
    color: root.normalColor
    border.width: 0
    border.color: "#80808080"
    radius: 0

    signal clicked(var mouse)
    signal doubleClicked(var mouse)
    signal rightClicked(var mouse)

    MouseArea {
        id: m_mousearea;
        anchors.fill: parent;
        preventStealing: true
        hoverEnabled: parent.enabled;
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onEntered: {
            if (!pressed)
                parent.state="Hovering";
        }
        onExited: {
            if (!pressed)
                parent.state = "Normal";
        }
        onClicked: {
            root.focus = true;
            if (mouse.button == Qt.RightButton) {
                root.rightClicked(mouse);
                // console.log("right clicked!");
            }
            else {
                root.clicked(mouse);
                // console.log("left clicked!");
            }
        }
        onDoubleClicked: {
            root.doubleClicked(mouse);
        }

        onPressed: {
            parent.state="Pressed";
        }
        onReleased: {
            if (containsMouse)
                parent.state="Hovering";
            else
                parent.state = "Normal";
        }
    }

    states: [
        State {
            name: "Hovering"
            PropertyChanges {
                target: root
                color: hoverColor
            }
            PropertyChanges {
                target: root
                border.width: 1
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                color: normalColor
            }
            PropertyChanges {
                target: root
                border.width: 0
            }
        },
        State {
            name: "Pressed"
            PropertyChanges {
                target: root
                color: pressedColor
            }
            PropertyChanges {
                target: root
                border.width: 1
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "Hovering"
            ColorAnimation { duration: 70 }
            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: 1.0
                duration: 100
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "*"; to: "Pressed"
            ColorAnimation { duration: 70 }

            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: root.scaleOnPressed? 1.0:1.0
                duration: 70
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "*"; to: "Normal"
            ColorAnimation { duration: 100 }
            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: 1.0
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    ]
}