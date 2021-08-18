import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts 1.15

Item {
    id: root
    property int fontSize: 16
    property alias model: crumbList.model

    IconButton {
        id: backButton
        iconSize: fontSize
        icon: "qrc:/assets/left.svg"
        flat: true
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width: 54
    }

    ListView {
        id: crumbList
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: backButton.right
        anchors.right: parent.right
        orientation: ListView.Horizontal
        height: parent.height

        delegate: PushButton {
            showIcon: true
            text: addrText
            height: crumbList.height
            width: ListView.view.width
            flat: true
            icon: "qrc:/assets/chevron_right.svg"
            iconSize: 16
        }
    }
}
