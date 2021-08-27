import QtQuick 2.15

Rectangle {
    id: root
    property int currentValue: 0
    property int dragValue: 0
    property bool onDragging: false
    property int maxValue: 10
    property double lineHeight: 2
    property int minValue: -10
    height: 16
    color: "transparent"

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    Behavior on lineHeight {
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
        anchors.verticalCenter: parent.verticalCenter
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
        x: ((root.width - 16) * (root.dragValue - root.minValue) / (root.maxValue - root.minValue))
        Behavior on x {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
    }

    Rectangle {
        id: sourcePoint
        anchors.verticalCenter: parent.verticalCenter
        width: 4
        height: 4
        radius: width / 2
        color: "#80808080"
        x: (root.width * Math.abs(root.minValue) / (root.maxValue - root.minValue)) - width / 2
    }

    Rectangle {
        id: currentPart
        anchors.verticalCenter: parent.verticalCenter
        height: lineHeight
        anchors.left: dragValue < 0 ? dragger.right : sourcePoint.right
        anchors.right: dragValue < 0 ? sourcePoint.left : dragger.left
        anchors.leftMargin: 4
        anchors.rightMargin: 4
        color: display.themeColor
        radius: height / 2
    }

    Rectangle {
        id: leftBackgroundPart
        anchors.verticalCenter: parent.verticalCenter
        height: lineHeight
        anchors.left: parent.left
        anchors.right: dragValue < 0 ? dragger.left : sourcePoint.left
        anchors.rightMargin: 4
        color: "#80808080"
        radius: height / 2
    }

    Rectangle {
        id: rightBackgroundPart
        anchors.verticalCenter: parent.verticalCenter
        height: lineHeight
        anchors.left: dragValue > 0 ? dragger.right : sourcePoint.right
        anchors.right: parent.right
        anchors.leftMargin: 4
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
            if (mouseX - 8 < 0)
                val = 0;
            else if (mouseX - 8 > root.width - 16)
                val = 1;
            else
                val = (mouseX - 8) / (root.width - 16);
            root.dragValue = Math.round(root.minValue + val * (root.maxValue - root.minValue));
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
                if (mouseX - 8 < 0)
                    val = 0;
                else if (mouseX - 8 > root.width - 16)
                    val = 1;
                else
                    val = (mouseX - 8) / (root.width - 16);
                root.dragValue = Math.round(root.minValue + val * (root.maxValue - root.minValue));
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
                lineHeight: 4
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
                lineHeight: 2
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
                lineHeight: 4
            }
        }
    ]
}
