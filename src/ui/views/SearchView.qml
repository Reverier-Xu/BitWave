import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: view
    color: Style.palette.window

    ScrollView {
        anchors.bottomMargin: 116
        anchors.fill: parent
        anchors.leftMargin: 32
        anchors.rightMargin: 32
        anchors.topMargin: 48

        Column {
            width: parent.width

            Label {
                id: title

                font.bold: true
                height: 36
                text: qsTr("Search Results")
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}