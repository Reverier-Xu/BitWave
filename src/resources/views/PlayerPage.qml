import QtQuick 2.15
import Reverier.MediaWidgets 1.0
import QtGraphicalEffects 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: settings.colorStyle? "white" : "black"

    VideoPlayer {
        id: videoPlayer
        anchors.fill: parent
    }

    Rectangle {
        id: musicPlayer
        anchors.fill: videoPlayer
        color: settings.colorStyle? "white" : "black"
        visible: !player.currentMediaIsVideo

        Rectangle {
            id: avatarContainer
            clip: true
            radius: 15
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: -height / 3
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: (parent.width - width * 2) / 5
            width: Math.min(parent.height, parent.width) / 2.5
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
                            radius: 24
                        }
                    }
                }
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
                visible: player.isLyricLoaded === 0
                font.pixelSize: 16
                color: "#e0808080"
            }

            Text {
                id: loadingLyricLabel
                text: qsTr("Loading Lyrics...")
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
                visible: player.isLyricLoaded === 1
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
            color: settings.colorStyle ? "#252525" : "#c0c0c0"
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
            color: settings.themeColor
            font.pixelSize: 16 
        }
    }
}
