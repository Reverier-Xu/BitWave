import QtQuick 2.15
import Reverier.MediaWidgets 1.0
import QtGraphicalEffects 1.15
import QtQuick.Particles 2.15
import QtQuick.Window 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "#1b1b1b"

    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    VideoPlayer {
        id: videoPlayer
        anchors.fill: parent

        Rectangle {
            id: videoPlayerLoaderContainer
            anchors.fill: parent
            color: display.colorStyle ? "#e0ffffff" : "#e01b1b1b"
            opacity: player.isMediaLoading ? 1 : 0

            MouseArea {
                id: pauseArea
                enabled: player.currentMediaIsVideo
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: display.mouseIsActive? Qt.ArrowCursor : Qt.BlankCursor

                onPositionChanged: {
                    display.delayedHide();
                }

                onClicked: {
                    if (player.isPlaying) {
                        player.pause();
                    } else {
                        player.resume();
                    }
                }

                onDoubleClicked: {
                    if (window.visibility === Window.FullScreen) {
                        window.showNormal();
                    } else {
                        window.showFullScreen();
                        display.sideBarExpanded = false;
                    }
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                }
            }

            Loader {
                id: videoLoader
                radius: 64
                anchors.centerIn: parent
                running: player.isMediaLoading && player.currentMediaIsVideo
            }
        }

        VideoIndicator {
            id: videoIndicator
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 42
            anchors.topMargin: 25
        }
    }

    Rectangle {
        id: musicPlayer
        anchors.fill: videoPlayer
        color: display.colorStyle? "white" : "#1b1b1b"

        Behavior on color {
            ColorAnimation {
                duration: 280
            }
        }
        visible: !player.currentMediaIsVideo

        Rectangle {
            id: avatarContainer
            clip: true
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: -height / 3
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: (parent.width - width * 2) / 5
            width: Math.min(parent.height, parent.width) / 2.5
            height: width
            color: "transparent"
            border.width: 3
            border.color: player.coverColor

            Behavior on border.color {
                ColorAnimation {
                    duration: 300
                }
            }
            radius: width / 2

            Image {
                id: avatar
                anchors.fill: parent
                anchors.margins: parent.width / 15
                smooth: true
                mipmap: true
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
                            radius: width / 2
                        }
                    }
                }

                PropertyAnimation {
                    property: "rotation"
                    target: avatar
                    from: 0
                    to: 360
                    duration: 30000
                    running: true
                    paused: !player.currentMediaIsVideo && !player.isPlaying
                    loops: Animation.Infinite
                }
            }
        }

        Rectangle {
            id: loadingCover
            anchors.fill: avatarContainer
            opacity: player.isMediaLoading? 1 : 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                }
            }
            radius: width / 2
            color: display.colorStyle ? "#e0ffffff" : "#e01b1b1b"

            Loader {
                id: musicAvatarLoader
                radius: parent.width / 5
                anchors.centerIn: parent
                running: player.isMediaLoading
            }
        }

        LyricsBox {
            id: lyricsBox
            objectName: "lyricsBox"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 32 + parent.height / 5
            anchors.bottomMargin: 100 + parent.height / 10
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: (parent.width - width * 2) / 10
            width: Math.min(parent.height, parent.width) / 2

            model: lyricsModel
            currentIndex: player.currentLyricIndex

            Text {
                id: noLyricLabel
                text: qsTr("No Lyrics\n\nAvailable")
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
                visible: !player.isLyricLoaded
                font.pixelSize: 16
                color: "#e0808080"
            }
        }

        TextMetrics {
            id: mediaTitleMetrics
            elide: Text.ElideRight
            font.pixelSize: 32
            text: player.currentMediaTitle
            elideWidth: parent.width / 3
        }

        Text {
            id: mediaTitle
            anchors.bottom: lyricsBox.top
            anchors.bottomMargin: 40
            anchors.horizontalCenter: lyricsBox.horizontalCenter
            font.pixelSize: 32
            text: mediaTitleMetrics.elidedText
            color: display.contentColor
            Behavior on color {
                ColorAnimation {
                    duration: 280
                }
            }
        }

        TextMetrics {
            id: mediaArtistAndAlbumMetrics
            elide: Text.ElideRight
            font.pixelSize: 16
            text: player.currentMediaArtist + " - " + player.currentMediaAlbum
            elideWidth: parent.width / 2.5
        }

        Text {
            id: mediaArtistAndAlbum
            anchors.top: mediaTitle.bottom
            anchors.horizontalCenter: mediaTitle.horizontalCenter
            elide: Text.ElideLeft
            text: mediaArtistAndAlbumMetrics.elidedText
            color: display.themeColor
            font.pixelSize: 16
        }
    }

    DropArea {
        anchors.fill: parent
        onDropped: {
            // TODO: should handle multiple files
            if (drop.hasUrls) {
                // for(var i = 0; i < drop.urls.length; i++) {
                //     // console.log(drop.urls.length);
                //     queue.playExternMedia(drop.urls[i]);
                // }
                queue.addExternMedia(drop.urls[0]);
            }
        }
    }
}
