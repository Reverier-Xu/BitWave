import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "#1b1b1b"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    TextLabel {
        id: settingsPageTitle
        text: qsTr("Program Settings")
        fontSize: 24
        iconSize: 24
        showIcon: true
        icon: "qrc:/assets/settings.svg"
        anchors.top: parent.top
        anchors.topMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 25
        height: 48
    }
    
    Flickable {
        contentWidth: parent.width
        contentHeight: endLine.y + 25
        anchors.top: settingsPageTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        
        clip: true

        ScrollBar.vertical: UXScrollBar { }

        Rectangle {
            id: languageSettingItem
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 120
            color: "transparent"
            Rectangle {
                anchors.verticalCenter: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "#60808080"
            }
            height: 36
            TextLabel {
                id: languageLabel
                text: qsTr("Language")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                showIcon: true
                icon: "qrc:/assets/language.svg"
            }

            PushButton {
                id: languageValue
                text: display.language
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                flat: true
                showIcon: false
                onClicked: {
                    if (display.language === "en_US") {
                        display.language = "zh_CN"
                    } else {
                        display.language = "en_US"
                    }
                }
            }
        }

        Rectangle {
            id: themeSettingItem
            anchors.top: languageSettingItem.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 120
            color: "transparent"
            Rectangle {
                anchors.verticalCenter: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "#60808080"
            }
            height: 36
            TextLabel {
                id: themeLabel
                text: qsTr("Theme")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                showIcon: true
                icon: "qrc:/assets/theme.svg"
            }

            PushButton {
                id: colorStyleValue
                text: display.colorStyle ? qsTr("Light") : qsTr("Dark")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                flat: true
                showIcon: false
                onClicked: {
                    display.colorStyle = !display.colorStyle;
                }
            }
        }

        Rectangle {
            id: systemTrayStyleSettingItem
            anchors.top: themeSettingItem.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 120
            color: "transparent"
            height: 36
            Rectangle {
                anchors.verticalCenter: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "#60808080"
            }
            TextLabel {
                id: systemTrayStyleLabel
                text: qsTr("System Tray Style")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                showIcon: true
                icon: "qrc:/assets/communication.svg"
            }

            PushButton {
                id: systemTrayStyleValue
                text: display.flatSystemTray ? qsTr("Flat") : qsTr("Colorful")
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                flat: true
                showIcon: false
                onClicked: {
                    display.flatSystemTray = !display.flatSystemTray;
                }
            }
        }

        Text {
            id: copyrightLabel
            text: qsTr("Powered by @ Reverier-Xu <reverier.xu@outlook.com>,\nCopyright (c) 2022 Wootec Team.")
            color: "#808080"
            font.pixelSize: 16
            anchors.top: systemTrayStyleSettingItem.bottom
            anchors.topMargin: 36
            anchors.left: systemTrayStyleSettingItem.left
        }

        Rectangle {
            id: endLine
            anchors.top: copyrightLabel.bottom
            anchors.topMargin: 15
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0
            color: "transparent"
        }
    }
}
