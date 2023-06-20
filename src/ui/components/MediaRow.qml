import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import RxUI

Button {
    id: control
    flat: true

    property int index
    property string title
    property string artists
    property string album
    property int time
    property bool playing

    implicitHeight: 36
    height: 36

    function getTimeString(displayTime) {
        let secs = Math.floor(displayTime);
        let minutes = Math.floor(secs/60);
        secs = secs % 60;
        return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    Label {
        id: idLabel
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 48
        text: (control.index + 1).toString().padStart(3, '0')
        visible: !control.playing
        opacity: 0.6

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: indicator
        anchors.centerIn: idLabel
        visible: false
        source: "qrc:/qt/qml/RxUI/assets/chevron-double-right.svg"
    }

    MultiEffect {
        anchors.fill: indicator
        source: indicator
        colorizationColor: Style.primary
        colorization: 1
        brightness: 0.5
        visible: control.playing
    }

    Label {
        id: timeLabel
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 64
        text: getTimeString(control.time)
        color: control.playing ? Style.primary : Style.palette.text

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Label {
        id: titleLabel
        anchors.left: idLabel.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: (parent.width - idLabel.width - timeLabel.width) / 3 - 12

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHLeft
        text: control.title
    }

    Label {
        id: artistLabel
        anchors.left: titleLabel.right
        anchors.leftMargin: 12
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: (parent.width - idLabel.width - timeLabel.width) / 3 - 12
        opacity: 0.6

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHLeft
        text: control.artists
    }

    Label {
        id: albumLabel
        anchors.left: artistLabel.right
        anchors.leftMargin: 12
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: (parent.width - idLabel.width - timeLabel.width) / 3 - 12
        opacity: 0.6

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHLeft
        text: control.album
    }

    Rectangle {
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: control.playing ? Color.transparent(Style.primary, 0.6) : Style.palette.mid
    }
}
