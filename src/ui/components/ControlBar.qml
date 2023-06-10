import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import RxUI
// import RxUI.Models

Rectangle {
    id: control

    color: Color.transparent(Style.palette.window, 0.8)
    height: 100

    Rectangle {
        id: coverContainer

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.top: parent.top
        anchors.topMargin: 16
        color: Style.palette.midlight
        radius: 12
        width: height

        Image {
            id: cover

            anchors.fill: parent
            antialiasing: true
            mipmap: true
            smooth: true
            source: player.coverPath
            visible: false
        }
        MultiEffect {
            anchors.fill: cover
            maskEnabled: true
            maskSpreadAtMin: 0
            maskThresholdMin: 0
            source: cover

            maskSource: ShaderEffectSource {
                height: cover.height
                width: cover.width

                sourceItem: Rectangle {
                    color: "white"
                    height: cover.height
                    radius: 12
                    width: cover.width
                }
            }
        }
    }
    Label {
        id: title

        anchors.bottom: parent.verticalCenter
        anchors.left: coverContainer.right
        anchors.leftMargin: 16
        anchors.right: previousButton.left
        anchors.rightMargin: 16
        font.bold: true
        height: parent.height / 3.5
        text: player.media.title
        verticalAlignment: Text.AlignVCenter

        ToolTip {
            parent: title
            text: player.media.title
            visible: title.hovered
            x: 0
        }
    }
    Label {
        id: artistAndAlbum

        anchors.left: title.left
        anchors.right: title.right
        anchors.top: parent.verticalCenter
        height: parent.height / 3.5
        opacity: 0.6
        text: `${player.media.artists.join(', ')} - ${player.media.album}`
        verticalAlignment: Text.AlignVCenter

        ToolTip {
            parent: artistAndAlbum
            text: `${player.media.artists.join(', ')} - ${player.media.album}`
            visible: artistAndAlbum.hovered
            x: 0
        }
    }
    TimeButton {
        id: previousButton

        anchors.right: pauseButton.left
        anchors.rightMargin: 16
        anchors.verticalCenter: pauseButton.verticalCenter
        icon.source: "qrc:/qt/qml/RxUI/assets/previous.svg"
        tempTime: progressBar.currentValue
    }
    PauseButton {
        id: pauseButton

        anchors.centerIn: parent
        objectName: "pauseButton"
        showTime: progressBar.onDragging
        tempTime: progressBar.dragValue
        width: 64

        onClicked: player.togglePause()
    }
    TimeButton {
        id: nextButton

        anchors.left: pauseButton.right
        anchors.leftMargin: 16
        anchors.verticalCenter: pauseButton.verticalCenter
        icon.source: "qrc:/qt/qml/RxUI/assets/next.svg"
        tempTime: progressBar.totalValue
    }
    Button {
        id: queueButton

        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 48
        icon.source: "qrc:/qt/qml/RxUI/assets/list.svg"
        radius: width / 2
        width: 48
    }
    VolumeButton {
        id: volumeButton

        anchors.right: queueButton.left
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        height: 48
        width: 48
    }
    Button {
        id: optionButton

        anchors.right: volumeButton.left
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 48
        icon.source: "qrc:/qt/qml/RxUI/assets/options.svg"
        radius: width / 2
        width: 48
    }
    Button {
        id: modeButton

        anchors.right: optionButton.left
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        height: 48
        icon.source: "qrc:/qt/qml/RxUI/assets/play-random.svg"
        radius: width / 2
        width: 48
    }
    InteractiveProgressBar {
        id: progressBar

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.top
        currentValue: player.currentTime
        height: 24
        totalValue: player.totalTime

        onEndDragging: function (time) {
            player.seek(time);
        }
    }
}
