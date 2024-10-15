import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: control

    color: Qt.alpha(Style.palette.window, 0.95)

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

    ComboBox {
        id: deviceComboBox

        anchors.top: title.bottom
        anchors.topMargin: 16
        anchors.left: title.left
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 32
        currentIndex: {
            // player.audioDevice is name
            let index = player.audioDeviceList.findIndex(function (item) {
                // console.log(item.name, player.audioDevice);
                return item.name === player.audioDevice;
            });
            return index;
        }
        flat: false
        model: player.audioDeviceList
        textRole: "description"
        valueRole: "name"
        onActivated: {
            // console.log(deviceComboBox.currentValue.name);
            player.audioDevice = deviceComboBox.currentValue;
        }
    }
}
