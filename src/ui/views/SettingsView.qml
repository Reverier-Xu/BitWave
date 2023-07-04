import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: view

    color: Style.palette.window

    ScrollView {
        id: container

        anchors.bottomMargin: 116
        anchors.fill: parent
        anchors.leftMargin: 32
        anchors.rightMargin: 32
        anchors.topMargin: 48

        Column {
            spacing: 6

            Label {
                id: title

                font.bold: true
                height: 36
                text: qsTr("Settings")
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                color: "transparent"
                height: 36
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    height: 36
                    text: qsTr("Language")
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: languageComboBox

                    anchors.right: parent.right
                    currentIndex: {
                        if (ui.language === "en_US") {
                            return 0;
                        } else if (ui.language === "zh_CN") {
                            return 1;
                        }
                    }
                    flat: true
                    model: [{
                            "text": "English",
                            "value": "en_US"
                        }, {
                            "text": "简体中文",
                            "value": "zh_CN"
                        }]
                    textRole: "text"
                    valueRole: "value"

                    onActivated: {
                        ui.language = languageComboBox.currentValue;
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }
            Rectangle {
                color: "transparent"
                height: 36
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    height: 36
                    text: qsTr("Color Theme")
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox {
                    id: themeComboBox

                    anchors.right: parent.right
                    currentIndex: {
                        if (Style.isDark) {
                            return 1;
                        } else {
                            return 0;
                        }
                    }
                    flat: true
                    model: [{
                            "text": qsTr("Light"),
                            "value": false
                        }, {
                            "text": qsTr("Dark"),
                            "value": true
                        }]
                    textRole: "text"
                    valueRole: "value"

                    onActivated: {
                        Style.isDark = themeComboBox.currentValue;
                        ui.colorStyle = Style.isDark;
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }
        }
    }
}