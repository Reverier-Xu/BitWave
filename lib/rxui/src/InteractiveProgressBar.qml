import QtQuick
import RxUI

Rectangle {
    id: root

    property double currentValue: 300
    property double dragValue: 0
    property double lineHeight: 2
    property bool onDragging: false
    property double totalValue: 1000
    property bool showHandle: true

    signal endDragging(double finalTime)
    signal startDragging()

    clip: true
    color: "transparent"
    states: [
        State {
            name: "Hovering"

            PropertyChanges {
                pointColor: Style.primary
                target: currentProgressPoint
            }

            PropertyChanges {
                lineHeight: 4
                target: root
            }

        },
        State {
            name: "Normal"

            PropertyChanges {
                pointColor: Style.palette.window
                target: currentProgressPoint
            }

            PropertyChanges {
                lineHeight: 2
                target: root
            }

        },
        State {
            name: "Pressed"

            PropertyChanges {
                target: root
            }

            PropertyChanges {
                pointColor: Style.primary
                target: currentProgressPoint
            }

            PropertyChanges {
                lineHeight: 4
                target: root
            }

        }
    ]

    Rectangle {
        id: progressPart

        anchors.left: parent.left
        anchors.right: onDragging ? (currentValue > dragValue ? dragPoint.left : currentProgressPoint.left) : currentProgressPoint.left
        anchors.verticalCenter: parent.verticalCenter
        color: Style.primary
        height: root.lineHeight
        radius: root.showHandle ? height / 2 : 0
    }

    Rectangle {
        id: differPart

        anchors.left: currentValue > dragValue ? dragPoint.right : currentProgressPoint.right
        anchors.right: currentValue > dragValue ? currentProgressPoint.left : dragPoint.left
        anchors.verticalCenter: parent.verticalCenter
        color: Style.warning
        height: root.lineHeight
        radius: root.showHandle ? height / 2 : 0
        visible: root.onDragging
    }

    Rectangle {
        id: unExplorePart

        anchors.left: onDragging ? (currentValue > dragValue ? currentProgressPoint.right : dragPoint.right) : currentProgressPoint.right
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        color: Style.palette.dark
        height: root.lineHeight
        radius: root.showHandle ? height / 2 : 0
    }

    Rectangle {
        id: currentProgressPoint

        property color pointColor: Style.palette.window

        anchors.verticalCenter: parent.verticalCenter
        color: "transparent"
        height: 16
        width: 24
        x: (root.width - 16) * (totalValue > 0 ? (currentValue / totalValue) : 0) - 4

        Rectangle {
            height: 16
            width: 16
            radius: 8
            anchors.centerIn: parent
            border.color: Style.primary
            border.width: 4
            color: currentProgressPoint.pointColor
            visible: root.showHandle
        }

        Rectangle {
            height: root.lineHeight
            anchors.right: parent.horizontalCenter
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            color: Style.primary
            visible: !root.showHandle
        }

        Rectangle {
            height: root.lineHeight
            anchors.right: parent.right
            anchors.left: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: Style.palette.dark
            visible: !root.showHandle
        }

        Behavior on x {
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutQuad
            }

        }

    }

    Rectangle {
        id: dragPoint

        anchors.verticalCenter: parent.verticalCenter
        height: 16
        visible: root.onDragging
        width: 24
        x: (root.width - 16) * (totalValue > 0 ? (dragValue / totalValue) : 0) - 4
        color: "transparent"

        Rectangle {
            height: 16
            width: 16
            radius: 8
            anchors.centerIn: parent
            color: Style.warning
        }

    }

    MouseArea {
        id: hoverArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: {
            if (!pressed)
                parent.state = "Hovering";

        }
        onExited: {
            if (!pressed)
                parent.state = "Normal";

        }
        onPositionChanged: {
            if (root.onDragging) {
                let val;
                if (mouseX - 8 < 0)
                    val = 0;
                else if (mouseX - 8 > root.width - 16)
                    val = totalValue;
                else
                    val = ((mouseX - 8) / (root.width - 16)) * totalValue;
                root.dragValue = val;
            }
        }
        onPressed: {
            root.state = "Pressed";
            root.onDragging = true;
            if (root.onDragging) {
                let val;
                if (mouseX - 8 < 0)
                    val = 0;
                else if (mouseX - 8 > root.width - 16)
                    val = totalValue;
                else
                    val = ((mouseX - 8) / (root.width - 16)) * totalValue;
                root.dragValue = val;
            }
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
    }

    Behavior on color {
        ColorAnimation {
            duration: 200
        }

    }

    Behavior on lineHeight {
        NumberAnimation {
            duration: 80
        }

    }

}
