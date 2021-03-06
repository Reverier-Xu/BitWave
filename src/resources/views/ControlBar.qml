import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "qrc:/components"

Rectangle {
    id: root
    height: 104
    opacity: display.mouseIsActive ? 1 : 0
    color: display.colorStyle? "#a0ffffff":"#d01b1b1b"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
    Behavior on opacity {
        NumberAnimation {
            duration: 100
        }
    }

    Rectangle {
        id: staticArea
        anchors.fill: parent
        anchors.topMargin: 12
        color: "transparent"

        Rectangle {
            id: avatarContainer
            clip: true
            radius: 6
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
                mipmap: true
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: avatar.width
                        height: avatar.height
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width
                            height: parent.height
                            radius: 6
                        }
                    }
                }
            }
        }

        Rectangle {
            id: loadingCover
            anchors.fill: avatarContainer
            opacity: player.isMediaLoading ? 1 : 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                }
            }

            color: display.colorStyle ? "#e0ffffff" : "#e01b1b1b"
            Loader {
                id: loader
                radius: parent.width / 4
                anchors.centerIn: parent
                running: player.isMediaLoading
            }
        }

        TextMetrics {
            id: mediaTitleMetrics
            elide: Text.ElideRight
            font.pixelSize: 16
            text: player.currentMediaTitle
            elideWidth: mediaTitle.width
        }

        Text {
            id: mediaTitle
            anchors.left: avatarContainer.right
            anchors.leftMargin: 18
            anchors.right: previousButton.left
            anchors.rightMargin: 25
            anchors.bottom: avatarContainer.verticalCenter
            anchors.bottomMargin: (parent.height / 2 - height) / 4
            color: display.contentColor
            font.pixelSize: 16
            text: mediaTitleMetrics.elidedText
        }

        TextMetrics {
            id: mediaArtistMetrics
            elide: Text.ElideRight
            font.pixelSize: 16
            text: player.currentMediaArtist
            elideWidth: mediaArtist.width
        }

        Text {
            id: mediaArtist
            anchors.left: avatarContainer.right
            anchors.leftMargin: 18
            anchors.right: previousButton.left
            anchors.rightMargin: 25
            anchors.top: avatarContainer.verticalCenter
            anchors.topMargin: (parent.height / 2 - height) / 4
            color: "#808080"
            font.pixelSize: 16
            text: mediaArtistMetrics.elidedText
        }

        ControlButton {
            id: playControlButton
            width: 64
            height: 64
            anchors.centerIn: parent
            displayActive: progressBar.onDragging
            displayTime: progressBar.dragTime
            icon: player.isPlaying? "qrc:/assets/pause.svg" : "qrc:/assets/play.svg"

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
            onClicked: {
                queue.userPreviousRequested();
            }
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
            onClicked: {
                queue.userNextRequested();
            }
        }

        IconButton {
            id: playModeButton
            width: 42
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: toolBarButton.left
            anchors.rightMargin: 15
            radius: width / 2
            iconSize: 24
            flat: true
            icon: queue.playModeIcon

            onClicked: {
                queue.changeMode();
            }
        }

        IconButton {
            id: toolBarButton
            width: 42
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: muteButton.left
            anchors.rightMargin: 15
            radius: width / 2
            iconSize: 24
            flat: true
            icon: "qrc:/assets/options.svg"

            onClicked: {
                if (display.queueBarIndex == 0 && display.queueBarExpanded) {
                    display.queueBarExpanded = false;
                } else {
                    if (display.queueBarIndex != 0) {
                        display.queueBarIndex = 0;
                    }
                    display.queueBarExpanded = true;
                }
            }
        }

        CircleSliderButton {
            id: muteButton
            width: 42
            height: width
            flat: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: queueButton.left
            anchors.rightMargin: 15
            radius: width / 2
            progress: player.volume
            icon: "qrc:/assets/volume-" + (player.isMuted? "0" : Math.floor(player.volume*100/34 + 1).toString()) + ".svg"
            iconSize: 24
            onClicked: {
                player.isMuted = !player.isMuted;
            }
            onWheel: {
                // console.log(wheel.angleDelta.y);
                player.volume = player.volume + wheel.angleDelta.y / 1200;
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

            onClicked: {
                if (display.queueBarIndex == 1 && display.queueBarExpanded) {
                    display.queueBarExpanded = false;
                } else {
                    if (display.queueBarIndex != 1) {
                        display.queueBarIndex = 1;
                    }
                    display.queueBarExpanded = true;
                }
            }
        }
    }

    ProgressBar {
        id: progressBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 24
        totalTime: player.totalTime
        currentTime: player.currentTime
        onEndDragging: {
            player.handleUserSeekRequest(finalTime);
        }
    }

    HoverHandler {
        onHoveredChanged: {
            if (hovered) {
                // console.log("control hovered");
                display.blockDelayedHide();
            } else {
                // console.log("control unhovered");
                display.delayedHide();
            }
        }
    }
}
