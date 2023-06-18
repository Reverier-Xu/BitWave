import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import RxUI
import RxUI.MediaWidgets

Rectangle {
    id: view

    VideoPlayer {
        id: videoPlayer

        anchors.fill: parent
        objectName: "videoPlayer"

        // Cover the video player instead of replacing it.
        // If the video player is hidden at application start time,
        // MPV will crash due to OpenGL context lost.
        Rectangle {
            id: musicPlayer

            anchors.fill: parent
            color: Style.palette.window
            objectName: "musicPlayer"
            visible: player.media.type !== 1

            Rectangle {
                id: coverContainer

                anchors.left: parent.left
                anchors.leftMargin: (parent.width / 2 - width) / 1.7
                anchors.top: parent.top
                anchors.topMargin: (parent.height - height - 132) / 2.4 + 32
                border.color: colorize.color
                border.width: 4
                color: "transparent"
                height: width
                radius: width / 2
                width: Math.min(parent.width / 3.5, (parent.height - 132) / 1.5)

                Behavior on border.color {
                    ColorAnimation {
                        duration: 300
                    }
                }

                Image {
                    id: cover

                    anchors.fill: parent
                    anchors.margins: 16
                    antialiasing: true
                    mipmap: true
                    smooth: true
                    source: player.coverPath
                    sourceSize: parent.size
                    visible: false
                }
                MultiEffect {
                    id: clippedCover

                    anchors.fill: cover
                    maskEnabled: true
                    maskSpreadAtMin: 1
                    maskSpreadAtMax: 1
                    maskThresholdMin: 0.5
                    source: cover

                    maskSource: ShaderEffectSource {
                        height: cover.height
                        width: cover.width

                        sourceItem: Rectangle {
                            color: "white"
                            height: cover.height
                            radius: width / 2
                            width: cover.width
                        }
                    }

                    PropertyAnimation {
                        duration: 30000
                        from: 0
                        loops: Animation.Infinite
                        paused: !player.playing || (player.media.type === 1)
                        property: "rotation"
                        running: true
                        target: clippedCover
                        to: 360
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: Color.transparent(Style.palette.window, 0.60)
                    opacity: (player.loading || player.coverLoading) ? 1 : 0
                    radius: width / 2
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 300
                            easing.type: Easing.OutExpo
                        }
                    }

                    Loader {
                        id: loader
                        radius: 32
                        anchors.centerIn: parent
                        running: player.loading || player.coverLoading
                    }
                }
            }
            Rectangle {
                id: lyricBox

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 100
                anchors.left: coverContainer.right
                anchors.leftMargin: 64
                anchors.right: parent.right
                anchors.rightMargin: 96
                anchors.top: parent.top
                anchors.topMargin: 64
                color: "transparent"

                Label {
                    id: title

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    font.bold: true
                    font.pixelSize: 28
                    height: 48
                    horizontalAlignment: Text.AlignHCenter
                    text: player.media.title
                    verticalAlignment: Text.AlignVCenter

                    ToolTip {
                        parent: title
                        text: player.media.title
                        visible: title.hovered
                    }
                }
                Label {
                    id: artistAndAlbum

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: title.bottom
                    color: Style.primary
                    height: 32
                    horizontalAlignment: Text.AlignHCenter
                    text: `${player.media.artists.join(', ')} - ${player.media.album}`
                    verticalAlignment: Text.AlignVCenter

                    ToolTip {
                        parent: artistAndAlbum
                        text: `${player.media.artists.join(', ')} - ${player.media.album}`
                        visible: artistAndAlbum.hovered
                    }
                }
            }
        }
    }
}