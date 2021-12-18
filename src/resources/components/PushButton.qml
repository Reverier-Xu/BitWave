import QtQuick 2.15
import QtGraphicalEffects 1.15

PushArea {
    id: root
    property int iconSize: 16
    property int fontSize: 16
    property alias icon: image.source
    property bool showIcon: true
    property bool noOverlay: false
    property alias text: buttonText.text
    property color contentColor: enabled ? display.contentColor : "#808080"
    property bool bold: false

    width: buttonText.width +root.fontSize * 1.4 + (root.showIcon? root.iconSize+root.fontSize*0.5:0)

    Image {
        id: image
        width: root.iconSize
        height: root.iconSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: root.fontSize * 0.7
        sourceSize: Qt.size(iconSize, iconSize)
        smooth: true
        antialiasing: true
        visible: root.noOverlay && root.showIcon
    }

    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        color: contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        smooth: true
        antialiasing: true
        visible: !root.noOverlay && root.showIcon
    }

    Text {
        id: buttonText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.showIcon? image.right: undefined
        anchors.leftMargin: root.fontSize * 0.5
        anchors.horizontalCenter: root.showIcon? undefined: parent.horizontalCenter
        color: contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        font.pixelSize: root.fontSize
        font.bold: root.bold
    }
}