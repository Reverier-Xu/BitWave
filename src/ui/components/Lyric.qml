import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: control

    property string content: ""
    property string translation: ""
    property bool isCurrent: false
    property double seekTime: 0

    color: "transparent"
    height: textItem.contentHeight + 16

    Button {
        id: timeIndicator

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: 64
        flat: true
        hoverColor: "transparent"
        pressedColor: "transparent"
        display: AbstractButton.TextOnly
        // icon.source: "qrc:/qt/qml/RxUI/assets/play.svg"
        text: {
            let secs = Math.floor(seekTime);
            let minutes = Math.floor(secs / 60);
            secs = secs % 60;
            return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
        }
        opacity: hoverHandler.hovered ? 0.6 : 0
        onClicked: {
            player.seek(seekTime);
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    Rectangle {
        id: background

        anchors.fill: textItem
        color: hoverHandler.hovered ? Style.palette.dark : "transparent"
        opacity: 0.3
        anchors.margins: -4
    }

    TextEdit {
        id: textItem

        readOnly: true
        text: content + (translation.length > 0 ? "\n" + translation : "")
        font.bold: control.isCurrent
        anchors.left: timeIndicator.right
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
        wrapMode: TextEdit.Wrap
        // font.bold: root.isActive
        color: Style.palette.text
        opacity: control.isCurrent ? 1 : 0.4
        selectByMouse: true
        selectedTextColor: Style.palette.text
        selectionColor: Color.transparent(Style.primary, 0.4)

        MouseArea {
            id: mouseArea

            acceptedButtons: Qt.RightButton
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor
            hoverEnabled: true
            onClicked: {
                contentMenu.popup();
            }

            Menu {
                id: contentMenu

                MenuItem {
                    icon.source: "qrc:/qt/qml/RxUI/assets/copy.svg"
                    text: qsTr("Copy")
                    onTriggered: {
                        textItem.selectAll();
                        textItem.copy();
                        textItem.select(0, 0);
                    }
                }
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
    }

    HoverHandler {
        id: hoverHandler
    }
}
