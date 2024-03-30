import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import RxUI

Button {
    id: control

    property int mIndex
    property string title
    property string artists
    property string album
    property int time
    property bool playing
    property bool canDelete: true

    signal addToPlaylistClicked(int index, string playlistName)
    signal deleteClicked(int index)

    function getTimeString(displayTime) {
        let secs = Math.floor(displayTime);
        let minutes = Math.floor(secs / 60);
        secs = secs % 60;
        return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    flat: true
    implicitHeight: 36
    height: 36

    Label {
        id: idLabel

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 48
        text: (control.mIndex + 1).toString().padStart(3, '0')
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
        visible: !control.hovered
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

    Row {
        id: operationRow

        visible: control.hovered
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        Button {
            id: addToButton

            width: 36
            height: 36
            display: AbstractButton.IconOnly
            flat: true
            icon.source: "qrc:/qt/qml/RxUI/assets/add.svg"
            onClicked: {
                playlistsPopup.open();
            }

            ToolTip {
                parent: addToButton
                text: qsTr("Add to playlist")
                visible: addToButton.hovered
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
                            addToPlaylistClicked(control.mIndex, name);
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

        Button {
            id: deleteButton

            width: 36
            height: 36
            display: AbstractButton.IconOnly
            flat: true
            icon.source: "qrc:/qt/qml/RxUI/assets/delete.svg"
            icon.color: "#FF3333"
            visible: canDelete
            onClicked: {
                deleteClicked(control.mIndex);
            }

            ToolTip {
                parent: deleteButton
                text: qsTr("Remove from play queue")
                visible: deleteButton.hovered
            }

        }

    }

}
