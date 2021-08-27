import QtQuick 2.15

Rectangle {
    id: root
    property int currentValue: 0
    property int dragValue: 0
    property bool onDragging: false
    property int maxValue: 10
    property double lineWidth: 2
    property int minValue: -10
    width: 16
    color: "transparent"

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    Behavior on lineWidth {
        NumberAnimation {
            duration: 200
        }
    }

    onCurrentValueChanged: {
        root.dragValue = root.currentValue
    }

    signal startDragging();
    signal endDragging(int finalValue);

    Rectangle {
        id: dragger
        anchors.horizontalCenter: parent.horizontalCenter
        width: 16
        height: 16
        radius: width / 2
        border.width: 4
        border.color: display.themeColor
        color: "transparent"
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        y: (root.height * (root.maxValue - root.dragValue) / (root.maxValue - root.minValue)) - height / 2
        Behavior on y {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
    }

    Rectangle {
        id: sourcePoint
        anchors.horizontalCenter: parent.horizontalCenter
        width: 16
        height: 16
        radius: width / 2
        border.width: 4
        border.color: "#80808080"
        color: "transparent"
        y: (root.height * root.maxValue / (root.maxValue - root.minValue)) - height / 2
    }

    Rectangle {
        id: currentPart
        anchors.horizontalCenter: parent.horizontalCenter
        width: lineWidth
        anchors.top: dragValue > 0 ? dragger.bottom : sourcePoint.bottom
        anchors.bottom: dragValue > 0 ? sourcePoint.top : dragger.top
        anchors.topMargin: 4
        anchors.bottomMargin: 4
        color: display.themeColor
        radius: width / 2
    }

    Rectangle {
        id: topBackgroundPart
        anchors.horizontalCenter: parent.horizontalCenter
        width: lineWidth
        anchors.top: parent.top
        anchors.bottom: dragValue > 0 ? dragger.top : sourcePoint.top
        anchors.bottomMargin: 4
        color: "#80808080"
        radius: width / 2
    }

    Rectangle {
        id: bottomBackgroundPart
        anchors.horizontalCenter: parent.horizontalCenter
        width: lineWidth
        anchors.top: dragValue < 0 ? dragger.bottom : sourcePoint.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 4
        color: "#80808080"
        radius: width / 2
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent

        hoverEnabled: true

        onEntered: {
            if (!pressed)
                parent.state = "Hovering";
        }

        onExited: {
            if (!pressed)
                parent.state = "Normal";
        }

        onPressed: {
            root.state = "Pressed";
            root.onDragging = true;
            let val = 0;
            if (mouseY - 8 < 0)
                val = 0;
            else if (mouseY - 8 > root.height - 16)
                val = 1;
            else
                val = (mouseY - 8) / (root.height - 16);
            root.dragValue = Math.round(root.maxValue - val * (root.maxValue - root.minValue));
            root.startDragging();
        }

        onReleased: {
            if (containsMouse)
                parent.state = "Hovering";
            else
                parent.state = "Normal";
            root.onDragging = false;
            root.endDragging(root.dragValue);
        }

        onPositionChanged: {
            if (root.onDragging) {
                let val = 0;
                if (mouseY - 8 < 0)
                    val = 0;
                else if (mouseY - 8 > root.height - 16)
                    val = 1;
                else
                    val = (mouseY - 8) / (root.height - 16);
                root.dragValue = Math.round(root.maxValue - val * (root.maxValue - root.minValue));
            }
        }
    }

    states: [
        State {
            name: "Hovering"
            PropertyChanges {
                target: dragger
                color: border.color
            }
            PropertyChanges {
                target: root
                lineWidth: 4
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: dragger
                color: "transparent"
            }
            PropertyChanges {
                target: root
                lineWidth: 2
            }
        },
        State {
            name: "Pressed"
            PropertyChanges {
                target: dragger
                color: border.color
            }
            PropertyChanges {
                target: root
                lineWidth: 4
            }
        }
    ]
}
