import QtQuick 2.15

PushButton {
    id: root
    property bool isTabActive: false
    bold: isTabActive
    Rectangle {
        id: activeIndicator
        color: display.themeColor
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: root.state === "Normal" ? 5 : 0
        width: 3
        radius: root.state === "Normal" ? 1.5 : 0
        height: root.isTabActive ? (root.state === "Normal" ? parent.height - 8 : parent.height) : 0
        Behavior on height {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }
        Behavior on anchors.leftMargin {
            NumberAnimation {
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
    }
}
