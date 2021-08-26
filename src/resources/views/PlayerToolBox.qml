import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: "transparent"

    TextLabel {
        id: queueTitle
        text: qsTr("Player Options")
        icon: "qrc:/assets/options.svg"
        height: 32
        anchors.top: parent.top
        anchors.left: parent.left
    }

    IconButton {
        id: muteButton
        width: 42
        height: width
        flat: true
        anchors.top: queueTitle.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 15
        radius: width / 2
        icon: "qrc:/assets/volume-" + (player.isMuted? "0" : Math.floor(player.volume*100/34 + 1).toString()) + ".svg"
        iconSize: 24
        onClicked: {
            player.isMuted = !player.isMuted;
        }
    }

    SliderBar {
        id: volumeBar
        anchors.verticalCenter: muteButton.verticalCenter
        anchors.left: muteButton.right
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 25
        currentValue: player.volume

        onEndDragging: {
            player.volume = finalValue;
        }
    }
}
