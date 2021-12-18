import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "black"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
    
    Flickable {
        contentWidth: parent.width
        contentHeight: endLine.y + 25
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        ScrollBar.vertical: UXScrollBar { }

        TextLabel {
            id: settingsPageTitle
            text: qsTr("Program Settings")
            fontSize: 24
            showIcon: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.topMargin: 36
            height: 48
        }

        TextLabel {
            id: languageLabel
            text: qsTr("Language")
            anchors.top: settingsPageTitle.bottom
            anchors.topMargin: 15
            anchors.left: settingsPageTitle.left
            anchors.leftMargin: 25
            height: 32
            showIcon: true
            icon: "qrc:/assets/language.svg"
        }

        PushButton {
            id: languageValue
            text: display.language
            anchors.verticalCenter: languageLabel.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 50
            height: 32
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

        Rectangle {
            id: endLine
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0
            color: "transparent"
        }
    }
}
