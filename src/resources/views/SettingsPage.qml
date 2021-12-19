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
        anchors.topMargin: 32
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        
        clip: true

        ScrollBar.vertical: UXScrollBar { }

        TextLabel {
            id: settingsPageTitle
            text: qsTr("Program Settings")
            fontSize: 24
            iconSize: 24
            showIcon: true
            icon: "qrc:/assets/settings.svg"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.topMargin: 36
            height: 48
        }

        Rectangle {
            anchors.top: settingsPageTitle.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(parent.width, 800)
            color: "#10808080"
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
            id: endLine
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0
            color: "transparent"
        }
    }
}
