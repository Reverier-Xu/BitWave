import QtQuick 2.15

InputBox {
    id: root
    property string icon: "qrc:/assets/search.svg"
    property int iconSize: 16

    IconButton {
        id: searchButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.height
        icon: root.icon
        iconSize: root.iconSize
        flat: true

        onClicked: {
            root.inputFinished(root.inputText);
        }
    }
}
