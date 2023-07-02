import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: control
    property string content: ""
    property string translation: ""
    property bool isCurrent: false
    color: mouseArea.hovered ? Style.palette.mid : "transparent"

    height: textItem.contentHeight + 16

    TextEdit {
        id: textItem
        readOnly: true
        text: content + (translation.length > 0 ? "\n" + translation : "")
        font.bold: control.isCurrent
        anchors.left: parent.left
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

        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }

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
    }
}
