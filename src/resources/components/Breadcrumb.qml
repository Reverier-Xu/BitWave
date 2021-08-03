import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts 1.15

Item {
    id: root
    property int fontSize: 16

    IconButton {
        id: backButton
        iconSize: 16
        icon: "qrc:/assets/left.svg"
        flat: true
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width: 54
    }

    ListView {
        id: crumbList
    }
}
