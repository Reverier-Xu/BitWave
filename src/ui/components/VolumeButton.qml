import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: control

    color: "transparent"
    radius: width / 2

    Button {
        id: button

        anchors.fill: parent
        display: AbstractButton.IconOnly
        flat: true
        icon.source: player.muted ? "qrc:/qt/qml/RxUI/assets/speaker-mute.svg" : `qrc:/qt/qml/RxUI/assets/speaker-${Math.floor(player.volume / 34)}.svg`
        radius: width / 2

        onClicked: function () {
            volumePopup.open();
        }
    }
    Canvas {
        id: canvas

        property int arcWidth: 2
        property double radius: control.radius

        anchors.centerIn: parent
        height: 2 * radius
        opacity: button.hovered ? 1 : 0
        width: 2 * radius

        Behavior on opacity  {
            NumberAnimation {
                duration: 100
            }
        }

        onPaint: {
            let ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            let r = player.volume / 100;
            ctx.beginPath();
            if (player.muted)
                ctx.strokeStyle = Style.warning;
            else
                ctx.strokeStyle = Style.primary;
            ctx.lineWidth = arcWidth;
            ctx.arc(width / 2, height / 2, radius - arcWidth / 2, -Math.PI / 2, (r * 2 * Math.PI) - Math.PI / 2);
            ctx.stroke();
        }
    }
    Popup {
        id: volumePopup

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        height: 56
        padding: 0
        width: 160
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Button {
            id: muteButton

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            display: AbstractButton.IconOnly
            flat: true
            height: 36
            icon.source: player.muted ? "qrc:/qt/qml/RxUI/assets/speaker-mute.svg" : `qrc:/qt/qml/RxUI/assets/speaker-${Math.floor(player.volume / 34)}.svg`
            radius: width / 2
            width: 36

            onClicked: function () {
                player.toggleMute();
                canvas.requestPaint();
            }
        }
        Slider {
            id: volumeSlider

            anchors.left: muteButton.right
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            enabled: !player.muted
            from: 0
            orientation: Qt.Horizontal
            to: 100
            value: player.volume

            onMoved: function () {
                player.toggleVolume(value);
            }
        }
    }

    Connections {
        target: player
        function onVolumeChanged() {
            canvas.requestPaint();
        }
    }
}
