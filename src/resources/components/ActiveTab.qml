import QtQuick 2.15

PushButton {
    id: root
    property bool isTabActive: false
    Rectangle {
        id: activeIndicator
        color: settings.themeColor
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        width: 4
        radius: 2
        height: root.isTabActive? parent.height - 8 : 0
        Behavior on height {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }
    }
}
