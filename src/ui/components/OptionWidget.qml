import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: control
    color: Color.transparent(Style.palette.window, 0.95)

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        propagateComposedEvents: false
        hoverEnabled: true
        preventStealing: true
    }

    Item {
        id: title
        height: 36
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 32
        anchors.right: parent.right
        anchors.rightMargin: 32

        Label {
            anchors.left: parent.left
            text: qsTr("Player Options")
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}