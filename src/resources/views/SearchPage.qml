import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "#1b1b1b"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    Item {
        id: contentItem
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 32
        anchors.bottomMargin: 100
        width: Math.min(parent.width, 1200)

        TextLabel {
            id: title
            fontSize: 24
            iconSize: 24
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.topMargin: 36
            height: 48
            text: qsTr("Search Results")
            showIcon: true
            icon: "qrc:/assets/search.svg"
        }

        CheckBox {
            id: localSearchCheckBox
            anchors.right: parent.right
            anchors.rightMargin: 48
            anchors.verticalCenter: title.verticalCenter
            text: qsTr("Local")
            chose: service.localSearchEnabled
            onClicked: {
                service.localSearchEnabled = !service.localSearchEnabled
            }
        }

        CheckBox {
            id: onlineSearchCheckBox
            anchors.right: localSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: qsTr("Online")
            chose: service.onlineSearchEnabled
            onClicked: {
                service.onlineSearchEnabled = !service.onlineSearchEnabled
            }
        }

        CheckBox {
            id: videoSearchCheckBox
            anchors.right: onlineSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: qsTr("Video")
            chose: service.videoSearchEnabled
            onClicked: {
                service.videoSearchEnabled = !service.videoSearchEnabled
            }
        }

        CheckBox {
            id: musicSearchCheckBox
            anchors.right: videoSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: qsTr("Music")
            chose: service.musicSearchEnabled
            onClicked: {
                service.musicSearchEnabled = !service.musicSearchEnabled
            }
        }
    }

}
