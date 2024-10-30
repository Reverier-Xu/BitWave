import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RxUI

Pane {
    id: control

    background: Rectangle {
        color: Qt.alpha(Style.palette.window, 0.95)
    }

    padding: 32
    topPadding: 16
    bottomPadding: 16

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        propagateComposedEvents: false
        hoverEnabled: true
        preventStealing: true
    }

    ColumnLayout {
        spacing: 16
        anchors.fill: parent

        Item {
            height: 36
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent

                Label {
                    Layout.fillWidth: true
                    text: qsTr("Player Options")
                    font.bold: true
                }

                ComboBox {
                    id: deviceComboBox
                    Layout.fillWidth: true
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
        }
        
        Item {
            Layout.fillHeight: true
        }
    }
}
