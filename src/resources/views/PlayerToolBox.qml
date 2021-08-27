import QtQuick 2.15
import QtQuick.Controls 2.15
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

    Flickable {
        anchors.top: queueTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ScrollBar.vertical: UXScrollBar { }

        TextLabel {
            id: volumeTipsLabel
            text: "VOLUME"
            fontSize: 18
            bold: true
            showIcon: false
            contentColor: display.contentColor
            height: 32
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 15
        }

        IconButton {
            id: muteButton
            width: 42
            height: width
            flat: true
            anchors.top: volumeTipsLabel.bottom
            anchors.topMargin: 6
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

        TextLabel {
            id: playModeTipsLabel
            text: qsTr("PLAY MODE")
            showIcon: false
            fontSize: 18
            bold: true
            contentColor: display.contentColor
            height: 32
            anchors.top: muteButton.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 6
        }

        IconButton {
            id: randomPlayModeButton
            width: 42
            height: width
            contentColor: queue.playMode === 2 ? display.themeColor : display.contentColor
            border.width: 2
            radius: width / 2
            anchors.top: playModeTipsLabel.bottom
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            icon: "qrc:/assets/play-random.svg"
            iconSize: 24
            onClicked: {
                queue.playMode = 2;
            }
        }

        IconButton {
            id: repeatOnePlayModeButton
            width: 42
            height: width
            contentColor: queue.playMode === 1 ? display.themeColor : display.contentColor
            border.width: 2
            radius: width / 2
            anchors.top: playModeTipsLabel.bottom
            anchors.topMargin: 15
            anchors.right: randomPlayModeButton.left
            anchors.rightMargin: 15
            icon: "qrc:/assets/play-repeat-one.svg"
            iconSize: 24
            onClicked: {
                queue.playMode = 1;
            }
        }

        IconButton {
            id: repeatAllPlayModeButton
            width: 42
            height: width
            contentColor: queue.playMode === 0 ? display.themeColor : display.contentColor
            border.width: 2
            radius: width / 2
            anchors.top: playModeTipsLabel.bottom
            anchors.topMargin: 15
            anchors.right: repeatOnePlayModeButton.left
            anchors.rightMargin: 15
            icon: "qrc:/assets/play-repeat-all.svg"
            iconSize: 24
            onClicked: {
                queue.playMode = 0;
            }
        }

        IconButton {
            id: orderPlayModeButton
            width: 42
            height: width
            contentColor: queue.playMode === 3 ? display.themeColor : display.contentColor
            border.width: 2
            radius: width / 2
            anchors.top: playModeTipsLabel.bottom
            anchors.topMargin: 15
            anchors.left: randomPlayModeButton.right
            anchors.leftMargin: 15
            icon: "qrc:/assets/play-order.svg"
            iconSize: 24
            onClicked: {
                queue.playMode = 3;
            }
        }

        IconButton {
            id: reversePlayModeButton
            width: 42
            height: width
            contentColor: queue.playMode === 4 ? display.themeColor : display.contentColor
            border.width: 2
            radius: width / 2
            anchors.top: playModeTipsLabel.bottom
            anchors.topMargin: 15
            anchors.left: orderPlayModeButton.right
            anchors.leftMargin: 15
            icon: "qrc:/assets/play-reverse.svg"
            iconSize: 24
            onClicked: {
                queue.playMode = 4;
            }
        }

        TextLabel {
            id: equalizerTipsLabel
            text: qsTr("EQUALIZER")
            showIcon: false
            fontSize: 18
            bold: true
            contentColor: display.contentColor
            height: 32
            anchors.top: randomPlayModeButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
        }

        IconButton {
            id: lightnessButton
            width: 42
            height: width
            flat: true
            anchors.top: equalizerTipsLabel.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
            radius: width / 2
            icon: "qrc:/assets/sun.svg"
            iconSize: 24
        }

        HorizontalStepSlider {
            id: lightnessSlider
            anchors.verticalCenter: lightnessButton.verticalCenter
            anchors.left: lightnessButton.right
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 25
            currentValue: player.lightness
            maxValue: 100
            minValue: -100
            onEndDragging: {
                player.lightness = finalValue;
            }
        }


        // RadioButton {
        //     id: repeatAllPlayModeButton
        //     anchors.top: playModeTipsLabel.bottom
        //     flat: true
        //     height: 32
        //     anchors.left: parent.left
        //     anchors.leftMargin: 15
        //     anchors.right: parent.right
        //     anchors.rightMargin: 15
        //     anchors.topMargin: 5
        //     text: qsTr("Repeat All")
        //     icon: "qrc:/assets/play-repeat-all.svg"
        //     chose: queue.playMode === 0
        //     onClicked: {
        //         queue.playMode = 0;
        //     }
        // }

        // RadioButton {
        //     id: repeatOnePlayModeButton
        //     anchors.top: repeatAllPlayModeButton.bottom
        //     flat: true
        //     height: 32
        //     anchors.left: parent.left
        //     anchors.leftMargin: 15
        //     anchors.right: parent.right
        //     anchors.rightMargin: 15
        //     anchors.topMargin: 5
        //     text: qsTr("Repeat One")
        //     icon: "qrc:/assets/play-repeat-one.svg"
        //     chose: queue.playMode === 1
        //     onClicked: {
        //         queue.playMode = 1;
        //     }
        // }

        // RadioButton {
        //     id: randomPlayModeButton
        //     anchors.top: repeatOnePlayModeButton.bottom
        //     flat: true
        //     height: 32
        //     anchors.left: parent.left
        //     anchors.leftMargin: 15
        //     anchors.right: parent.right
        //     anchors.rightMargin: 15
        //     anchors.topMargin: 5
        //     text: qsTr("Random")
        //     icon: "qrc:/assets/play-random.svg"
        //     chose: queue.playMode === 2
        //     onClicked: {
        //         queue.playMode = 2;
        //     }
        // }

        // RadioButton {
        //     id: orderPlayModeButton
        //     anchors.top: randomPlayModeButton.bottom
        //     flat: true
        //     height: 32
        //     anchors.left: parent.left
        //     anchors.leftMargin: 15
        //     anchors.right: parent.right
        //     anchors.rightMargin: 15
        //     anchors.topMargin: 5
        //     text: qsTr("Play in order")
        //     icon: "qrc:/assets/play-order.svg"
        //     chose: queue.playMode === 3
        //     onClicked: {
        //         queue.playMode = 3;
        //     }
        // }

        // RadioButton {
        //     id: reversePlayModeButton
        //     anchors.top: orderPlayModeButton.bottom
        //     flat: true
        //     height: 32
        //     anchors.left: parent.left
        //     anchors.leftMargin: 15
        //     anchors.right: parent.right
        //     anchors.rightMargin: 15
        //     anchors.topMargin: 5
        //     text: qsTr("Reverse")
        //     icon: "qrc:/assets/play-reverse.svg"
        //     chose: queue.playMode === 4
        //     onClicked: {
        //         queue.playMode = 4;
        //     }
        // }
    }
}
