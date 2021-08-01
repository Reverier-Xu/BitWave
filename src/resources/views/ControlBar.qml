import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: "transparent"
    height: 100

    Rectangle {
        id: staticArea
        anchors.fill: parent
        anchors.topMargin: 8
        color: settings.colorStyle? "#a0ffffff":"#d0000000"

        Rectangle {
            id: avatarContainer
            clip: true
            radius: 15
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            width: 64
            height: width
            color: "transparent"

            Image {
                id: avatar
                anchors.fill: parent
                smooth: true
                source: player.currentMediaCover
                sourceSize: parent.size
                antialiasing: true
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: avatar.width
                        height: avatar.height
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width
                            height: parent.height
                            radius: 12
                        }
                    }
                }
            }
        }

        TextMetrics {
            id: mediaTitleMetrics
            elide: Text.ElideRight
            font.pixelSize: 32
            text: player.currentMediaTitle
            elideWidth: previousButton.x - avatarContainer.x - avatarContainer.width
        }

        TextLabel {
            id: mediaTitle
            anchors.left: avatarContainer.right
            anchors.leftMargin: 12
            anchors.top: avatarContainer.top
            anchors.bottom: avatarContainer.verticalCenter
            showIcon: false
            text: mediaTitleMetrics.elidedText
        }

        TextMetrics {
            id: mediaArtistMetrics
            elide: Text.ElideRight
            font.pixelSize: 32
            text: player.currentMediaArtist
            elideWidth: previousButton.x - avatarContainer.x - avatarContainer.width
        }

        TextLabel {
            id: mediaArtist
            anchors.left: avatarContainer.right
            anchors.leftMargin: 12
            anchors.top: avatarContainer.verticalCenter
            anchors.bottom: avatarContainer.bottom
            contentColor: "#808080"
            showIcon: false
            text: mediaArtistMetrics.elidedText
        }

        ControlButton {
            id: playControlButton
            width: 64
            height: 64
            anchors.centerIn: parent
            displayActive: progressBar.onDragging
            displayTime: progressBar.dragTime
            icon: player.isPlaying? "qrc:/assets/pause-large.svg" : "qrc:/assets/play-large.svg"

            onClicked: {
                if (player.isPlaying)
                    player.pause();
                else
                    player.resume();
            }
        }

        TimeButton {
            id: previousButton
            width: 72
            height: 48
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: playControlButton.left
            anchors.rightMargin: 15
            displayTime: player.currentTime
            icon: "qrc:/assets/previous.svg"
        }

        TimeButton {
            id: nextButton
            width: 72
            height: 48
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: playControlButton.right
            anchors.leftMargin: 15
            displayTime: player.totalTime
            icon: "qrc:/assets/next.svg"
        }

        IconButton {
            id: muteButton
            width: 42
            height: width
            flat: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: queueButton.left
            anchors.rightMargin: 124
            radius: width / 2
            icon: "qrc:/assets/volume-" + (player.isMuted? "0" : Math.floor(player.volume*100/34 + 1).toString()) + ".svg"
            iconSize: 24
            onClicked: {
                player.isMuted = !player.isMuted;
            }
        }

        SliderBar {
            id: volumeSlider
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: queueButton.left
            anchors.rightMargin: 12
            anchors.left: muteButton.right
            anchors.leftMargin: 6
            currentValue: player.volume
            onEndDragging: {
                player.volume = finalValue
            }
        }

        IconButton {
            id: queueButton
            width: 42
            height: width
            flat: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 25
            radius: width / 2
            icon: "qrc:/assets/list.svg"
            iconSize: 24
        }
    }

    ProgressBar {
        id: progressBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: staticArea.top
        totalTime: player.totalTime
        currentTime: player.currentTime
        onEndDragging: {
            player.userDragHandler(finalTime);
        }

        color: settings.colorStyle? "#a0ffffff":"#d0000000"

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "transparent"
            }
            GradientStop {
                position: 0.3
                color: settings.colorStyle? "#a0ffffff":"#d0000000"
            }
            GradientStop {
                position: 0.5
                color: settings.colorStyle? "#a0ffffff":"#d0000000"
            }
            GradientStop {
                position: 0.51
                color: "transparent"
            }
            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }
}
