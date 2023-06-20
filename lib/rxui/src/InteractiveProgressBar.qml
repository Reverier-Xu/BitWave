import QtQuick
import RxUI

Rectangle {
    id: root

    property double currentValue: 300
    property double dragValue: 0
    property double lineHeight: 2
    property bool onDragging: false
    property double totalValue: 1000

    signal endDragging(double finalTime)
    signal startDragging

    color: "transparent"

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }
    Behavior on lineHeight  {
        NumberAnimation {
            duration: 80
        }
    }
    states: [
        State {
            name: "Hovering"

            PropertyChanges {
                color: border.color
                target: currentProgressPoint
            }
            PropertyChanges {
                color: border.color
                target: dragPoint
            }
            PropertyChanges {
                lineHeight: 4
                target: root
            }
        },
        State {
            name: "Normal"

            PropertyChanges {
                color: Style.palette.window
                target: currentProgressPoint
            }
            PropertyChanges {
                color: Style.palette.window
                target: dragPoint
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
                color: border.color
                target: currentProgressPoint
            }
            PropertyChanges {
                border.color: Style.warning
                color: Style.warning
                target: dragPoint
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
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        color: Style.primary
        height: root.lineHeight
        radius: height / 2
    }
    Rectangle {
        id: differPart

        anchors.left: currentValue > dragValue ? dragPoint.right : currentProgressPoint.right
        anchors.leftMargin: 4
        anchors.right: currentValue > dragValue ? currentProgressPoint.left : dragPoint.left
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        color: Style.warning
        height: root.lineHeight
        radius: height / 2
        visible: root.onDragging
    }
    Rectangle {
        id: unExplorePart

        anchors.left: onDragging ? (currentValue > dragValue ? currentProgressPoint.right : dragPoint.right) : currentProgressPoint.right
        anchors.leftMargin: 4
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        color: Style.palette.dark
        height: root.lineHeight
        radius: height / 2
    }
    Rectangle {
        id: currentProgressPoint

        anchors.verticalCenter: parent.verticalCenter
        border.color: Style.primary
        border.width: 4
        color: Style.palette.window
        height: 16
        radius: 8
        width: 16
        x: (root.width - 16) * (totalValue > 0 ? (currentValue / totalValue) : 0)

        Behavior on x  {
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutQuad
            }
        }
    }
    Rectangle {
        id: dragPoint

        anchors.verticalCenter: parent.verticalCenter
        border.color: Style.warning
        border.width: 4
        color: Style.palette.window
        height: 16
        radius: 8
        visible: root.onDragging
        width: 16
        x: (root.width - 16) * (totalValue > 0 ? (dragValue / totalValue) : 0)
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
}
