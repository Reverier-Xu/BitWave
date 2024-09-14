import "../components"
import Qt.labs.platform as Platform
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

        Rectangle {
            id: videoTimeIndicator

            color: Color.transparent(Style.palette.window, 0.6)
            height: 36
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 12 + (ui.hideControls ? 0 : 36)
            anchors.rightMargin: 12
            radius: 4
            width: timeLabel.width + 32

            Label {
                id: timeLabel

                function currentDateTime() {
                    return Qt.formatDateTime(new Date(), "hh:mm");
                }

                anchors.centerIn: parent
                font.pixelSize: 24
                font.bold: true

                Timer {
                    id: timeLabelTimer

                    interval: 3
                    repeat: true
                    running: true
                    onTriggered: {
                        timeLabel.text = timeLabel.currentDateTime();
                    }
                }
            }

            Behavior on anchors.topMargin {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }
        }

        Rectangle {
            id: videoPlayerLoaderContainer

            anchors.fill: parent
            color: Style.palette.mid
            opacity: player.loading ? 1 : 0

            MouseArea {
                id: pauseArea

                enabled: player.media.type === 1
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: ui.hideControls ? Qt.BlankCursor : Qt.ArrowCursor
                onPositionChanged: {
                    ui.hideControls = false;
                    ui.autoHideControls();
                }
                onClicked: {
                    if (player.playing)
                        player.pause();
                    else
                        player.resume();
                }
                onDoubleClicked: {
                    if (window.visibility === Window.FullScreen) {
                        window.showNormal();
                    } else {
                        window.showFullScreen();
                        ui.sideBarExpanded = false;
                    }
                    if (player.playing)
                        player.pause();
                    else
                        player.resume();
                }
            }

            Loader {
                id: videoLoader

                radius: 64
                anchors.centerIn: parent
                running: player.loading
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                }
            }
        }
    }

    // Cover the video player instead of replacing it.
    // If the video player is hidden at application start time,
    // MPV will crash due to OpenGL context lost.
    Rectangle {
        id: musicPlayer

        anchors.fill: videoPlayer
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

            MouseArea {
                id: mouseArea

                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onClicked: {
                    contentMenu.popup();
                }

                Menu {
                    id: contentMenu

                    MenuItem {
                        icon.source: "qrc:/qt/qml/RxUI/assets/save.svg"
                        text: qsTr("Save Album Cover")
                        onTriggered: saveDialog.open()
                    }
                }
            }

            Platform.FileDialog {
                id: saveDialog

                fileMode: Platform.FileDialog.SaveFile
                title: qsTr("Save Album Cover")
                onAccepted: {
                    player.saveMediaCover(file);
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
                maskSpreadAtMax: 1
                maskSpreadAtMin: 1
                maskThresholdMin: 0.5
                source: cover

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
            }

            Rectangle {
                anchors.fill: parent
                color: Color.transparent(Style.palette.window, 0.6)
                opacity: (player.loading || player.coverLoading) ? 1 : 0
                radius: width / 2

                Loader {
                    id: loader

                    anchors.centerIn: parent
                    radius: 32
                    running: player.loading || player.coverLoading
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 300
                        easing.type: Easing.OutExpo
                    }
                }
            }

            Button {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                display: AbstractButton.IconOnly
                icon.source: "qrc:/qt/qml/RxUI/assets/heart.svg"
                flat: true
                onClicked: {
                    playlistsPopup.open();
                }

                Popup {
                    id: playlistsPopup

                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                    padding: 0
                    margins: 0
                    x: 0
                    y: parent.height
                    height: playlist.size * 40
                    width: 200

                    ListView {
                        id: listView

                        model: playlist
                        anchors.fill: parent
                        clip: true

                        delegate: Button {
                            text: name
                            height: 40
                            width: ListView.view.width
                            flat: true
                            alignment: Qt.AlignVCenter | Qt.AlignLeft
                            display: AbstractButton.TextBesideIcon
                            icon.source: "qrc:/qt/qml/RxUI/assets/star-line-horizontal-3.svg"
                            onClicked: {
                                queue.addToPlaylist(name);
                                playlistsPopup.close();
                            }
                        }
                    }

                    enter: Transition {
                        NumberAnimation {
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 120
                        }

                        NumberAnimation {
                            property: "height"
                            from: playlist.size * 20
                            to: playlist.size * 40
                            duration: 300
                            easing.type: Easing.OutExpo
                        }
                    }

                    exit: Transition {
                        NumberAnimation {
                            property: "opacity"
                            from: 1
                            to: 0
                            duration: 120
                        }

                        NumberAnimation {
                            property: "height"
                            from: playlist.size * 40
                            to: playlist.size * 20
                            duration: 300
                            easing.type: Easing.OutExpo
                        }
                    }
                }
            }

            Behavior on border.color {
                ColorAnimation {
                    duration: 300
                }
            }
        }

        Rectangle {
            id: lyricBox

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            anchors.left: coverContainer.right
            anchors.right: parent.right
            anchors.rightMargin: 96
            anchors.top: parent.top
            anchors.topMargin: 64
            color: "transparent"

            Label {
                id: title

                anchors.left: parent.left
                anchors.leftMargin: 64
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
                    y: 48
                }
            }

            Label {
                id: artistAndAlbum

                anchors.left: parent.left
                anchors.leftMargin: 64
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

            ListView {
                id: lyricsContent

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.top: artistAndAlbum.bottom
                anchors.topMargin: 8
                clip: true
                currentIndex: lyrics.currentIndex
                footerPositioning: ListView.InlineFooter
                headerPositioning: ListView.InlineHeader
                highlightMoveDuration: 300
                highlightMoveVelocity: -1
                highlightRangeMode: ListView.ApplyRange
                model: lyricsModel
                preferredHighlightBegin: 80
                preferredHighlightEnd: 80
                opacity: lyrics.loading ? 0 : 1

                Behavior on opacity {
                    NumberAnimation {
                        duration: 120
                    }
                }

                ScrollBar.vertical: ScrollBar {}

                delegate: Lyric {
                    content: lyricsContent
                    isCurrent: lyrics.currentIndex === lyricsId
                    translation: lyricsTranslation
                    seekTime: lyricsTime
                    width: ListView.view.width
                }

                footer: Item {
                    height: 64
                }

                header: Item {
                    height: 64
                }
            }

            Rectangle {
                anchors.fill: lyricsContent
                anchors.leftMargin: 64
                color: Color.transparent(Style.palette.window, 0.6)
                opacity: lyrics.loading ? 1 : 0
                radius: width / 2

                Loader {
                    anchors.centerIn: parent
                    radius: 24
                    running: lyrics.loading
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 120
                    }
                }
            }
        }
    }
}
