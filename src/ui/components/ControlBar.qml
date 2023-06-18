import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Layouts
import RxUI
// import RxUI.Models

Rectangle {
    id: control

    color: Color.transparent(Style.palette.window, 0.95)
    height: 100
    property bool queueVisible: false
    property bool optionVisible: false

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
            smooth: true
            antialiasing: true
            mipmap: true
            source: player.coverPath
            sourceSize: parent.size
            visible: false
        }
        MultiEffect {
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
                    radius: 12
                    width: cover.width
                    smooth: true
                    antialiasing: true
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: Color.transparent(Style.palette.window, 0.60)
            opacity: (player.loading || player.coverLoading) ? 1 : 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }

            Loader {
                id: loader
                radius: 12
                anchors.centerIn: parent
                running: player.loading || player.coverLoading
            }
        }
    }
    Label {
        id: title

        anchors.bottom: parent.verticalCenter
        anchors.left: coverContainer.right
        anchors.leftMargin: 16
        anchors.right: centerContainer.left
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

    RowLayout {
        id: centerContainer

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 16

        TimeButton {
            id: previousButton
            icon.source: "qrc:/qt/qml/RxUI/assets/previous.svg"
            tempTime: progressBar.currentValue

            onClicked: {
                queue.prev()
            }
        }
        PauseButton {
            id: pauseButton

            objectName: "pauseButton"
            showTime: progressBar.onDragging
            tempTime: progressBar.dragValue
            implicitWidth: 64
            implicitHeight: 64

            onClicked: player.togglePause()
        }
        TimeButton {
            id: nextButton
            icon.source: "qrc:/qt/qml/RxUI/assets/next.svg"
            tempTime: progressBar.totalValue

            onClicked: {
                queue.next()
            }
        }
    }


    RowLayout {
        id: rightContainer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        spacing: 8
        anchors.rightMargin: 24

        Button {
            id: modeButton

            display: AbstractButton.IconOnly
            flat: true
            icon.source: queue.modeIcon
            radius: width / 2
            implicitHeight: 48
            implicitWidth: 48

            onClicked: {
                queue.toggleChangeMode()
            }
        }

        VolumeButton {
            id: volumeButton

            implicitHeight: 48
            implicitWidth: 48
        }

        Rectangle {
            width: 1
            color: Style.palette.mid
            height: 16
        }

        Button {
            id: optionButton

            display: AbstractButton.IconOnly
            flat: !control.optionVisible
            implicitHeight: 48
            implicitWidth: 48
            icon.source: control.optionVisible ? "qrc:/qt/qml/RxUI/assets/chevron-right.svg" : "qrc:/qt/qml/RxUI/assets/options.svg"
            radius: width / 2
            rotation: control.optionVisible ? 90 : 0

            Behavior on rotation {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }

            onClicked: {
                if (control.queueVisible)
                    control.queueVisible = false
                control.optionVisible = !control.optionVisible
            }
        }

        Button {
            id: queueButton

            display: AbstractButton.IconOnly
            flat: !control.queueVisible
            implicitHeight: 48
            implicitWidth: 48
            icon.source: control.queueVisible ? "qrc:/qt/qml/RxUI/assets/chevron-right.svg" : "qrc:/qt/qml/RxUI/assets/list.svg"
            radius: width / 2
            rotation: control.queueVisible ? 90 : 0

            Behavior on rotation {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.OutExpo
                }
            }

            onClicked: {
                if (control.optionVisible)
                    control.optionVisible = false
                control.queueVisible = !control.queueVisible
            }
        }
    }

    InteractiveProgressBar {
        id: progressBar

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: control.queueVisible || control.optionVisible ? parent.bottom : parent.top
        currentValue: player.currentTime
        height: 24
        totalValue: player.totalTime

        onEndDragging: function (time) {
            player.seek(time);
        }
    }
}
