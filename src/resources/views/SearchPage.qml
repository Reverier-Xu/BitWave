import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "black"
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
            anchors.left: parent.left
            anchors.top: parent.top
            anchors. leftMargin: 32
            anchors.topMargin: 12
            height: 32
            text: "Search Results"
            showIcon: true
            icon: "qrc:/assets/search.svg"
        }

        CheckBox {
            id: localSearchCheckBox
            anchors.right: parent.right
            anchors.rightMargin: 48
            anchors.verticalCenter: title.verticalCenter
            text: "Local"
            chose: service.localSearchEnabled
            onClicked: {
                service.localSearchEnabled = !service.localSearchEnabled
            }
        }

        CheckBox {
            id: onlineSearchCheckBox
            anchors.right: localSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: "Online"
            chose: service.onlineSearchEnabled
            onClicked: {
                service.onlineSearchEnabled = !service.onlineSearchEnabled
            }
        }

        CheckBox {
            id: videoSearchCheckBox
            anchors.right: onlineSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: "Video"
            chose: service.videoSearchEnabled
            onClicked: {
                service.videoSearchEnabled = !service.videoSearchEnabled
            }
        }

        CheckBox {
            id: musicSearchCheckBox
            anchors.right: videoSearchCheckBox.left
            anchors.verticalCenter: title.verticalCenter
            text: "Music"
            chose: service.musicSearchEnabled
            onClicked: {
                service.musicSearchEnabled = !service.musicSearchEnabled
            }
        }
    }

}
