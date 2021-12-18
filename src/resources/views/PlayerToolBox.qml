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
        contentWidth: parent.width
        contentHeight: endLine.y + 25
        anchors.top: queueTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        ScrollBar.vertical: UXScrollBar { }

        TextLabel {
            id: volumeTipsLabel
            text: qsTr("VOLUME")
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
            onClicked: {
                player.lightness = 0;
            }
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

        IconButton {
            id: contrastButton
            width: 42
            height: width
            flat: true
            anchors.top: lightnessButton.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
            radius: width / 2
            icon: "qrc:/assets/theme.svg"
            iconSize: 24
            onClicked: {
                player.contrast = 0;
            }
        }

        HorizontalStepSlider {
            id: contrastSlider
            anchors.verticalCenter: contrastButton.verticalCenter
            anchors.left: contrastButton.right
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 25
            currentValue: player.contrast
            maxValue: 100
            minValue: -100
            onEndDragging: {
                player.contrast = finalValue;
            }
        }

        IconButton {
            id: saturationButton
            width: 42
            height: width
            flat: true
            anchors.top: contrastButton.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
            radius: width / 2
            icon: "qrc:/assets/saturation.svg"
            iconSize: 24
            onClicked: {
                player.saturation = 0;
            }
        }

        HorizontalStepSlider {
            id: saturationSlider
            anchors.verticalCenter: saturationButton.verticalCenter
            anchors.left: saturationButton.right
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 25
            currentValue: player.saturation
            maxValue: 100
            minValue: -100
            onEndDragging: {
                player.saturation = finalValue;
            }
        }

        IconButton {
            id: gammaButton
            width: 42
            height: width
            flat: true
            anchors.top: saturationButton.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
            radius: width / 2
            icon: "qrc:/assets/gamma.svg"
            iconSize: 24
            onClicked: {
                player.gamma = 0;
            }
        }

        HorizontalStepSlider {
            id: gammaSlider
            anchors.verticalCenter: gammaButton.verticalCenter
            anchors.left: gammaButton.right
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 25
            currentValue: player.gamma
            maxValue: 100
            minValue: -100
            onEndDragging: {
                player.gamma = finalValue;
            }
        }

        IconButton {
            id: hueButton
            width: 42
            height: width
            flat: true
            anchors.top: gammaButton.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
            radius: width / 2
            icon: "qrc:/assets/hue.svg"
            iconSize: 24
            onClicked: {
                player.hue = 0;
            }
        }

        HorizontalStepSlider {
            id: hueSlider
            anchors.verticalCenter: hueButton.verticalCenter
            anchors.left: hueButton.right
            anchors.right: parent.right
            anchors.leftMargin: 6
            anchors.rightMargin: 25
            currentValue: player.hue
            maxValue: 100
            minValue: -100
            onEndDragging: {
                player.hue = finalValue;
            }
        }

        TextLabel {
            id: displayTipsLabel
            text: qsTr("DISPLAY OPTIONS")
            showIcon: false
            fontSize: 18
            bold: true
            contentColor: display.contentColor
            height: 32
            anchors.top: hueButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
        }

        TextLabel {
            id: displayVideoIndicatorLabel
            text: qsTr("Show time")
            showIcon: false
            height: 32
            anchors.top: displayTipsLabel.bottom
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 15
        }

        Switch {
            id: displayVideoIndicatorSwitch
            anchors.verticalCenter: displayVideoIndicatorLabel.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 30
            isOn: display.showVideoTime
            onClicked: {
                display.showVideoTime = !display.showVideoTime;
            }
        }

        Rectangle {
            id: endLine
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: displayVideoIndicatorLabel.bottom
            anchors.topMargin: 25
            width: parent.width - 100
            height: 1
            color: "#80808080"
        }
    }
}
