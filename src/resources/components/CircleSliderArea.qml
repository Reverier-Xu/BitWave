import QtQuick 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: root
    property bool flat: false
    property bool scaleOnPressed: true
    property color normalColor: flat?"transparent":"#30808080"
    property color hoverColor: flat?"#20808080":"#40808080"
    property color pressedColor: flat?"#30808080":"#60808080"
    property color sliderColor: display.themeColor
    property double progress: 1.0
    
    color: root.normalColor
    border.width: 0
    border.color: "#80808080"
    radius: width / 2

    signal clicked(var mouse)
    signal doubleClicked(var mouse)
    signal rightClicked(var mouse)
    signal wheel(var wheel)

    state: "Normal"

    Canvas {
        property color arcColor: root.sliderColor
        property int arcWidth: 2
        property double radius: root.radius
        opacity: parent.state === "Hovering" ? 1 : 0
        
        Behavior on opacity {
            NumberAnimation {
                duration: 100
            }
        }

        Behavior on arcColor {
            ColorAnimation {
                duration: 100
            }
        }

        anchors.centerIn: parent

        id: canvas
        width: 2 * radius
        height: 2 * radius
    
        onPaint: {
            let ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            let r = root.progress;
            ctx.beginPath();
            ctx.strokeStyle = arcColor;
            ctx.lineWidth = arcWidth;

            ctx.arc(width / 2, height / 2 , radius - arcWidth / 2,
                        - Math.PI / 2, (r * 2 * Math.PI) - Math.PI / 2);
            ctx.stroke();
        }
    }

    onProgressChanged: {
        // console.log("request");
        canvas.requestPaint();
    }

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

        onWheel: {
            root.wheel(wheel);
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

    Behavior on color {
        ColorAnimation {
            duration: 100
        }
    }
}