import QtQuick 2.15
import QtGraphicalEffects 1.15

PushArea {
    id: root
    property int iconSize: 24
    property int fontSize: 16
    property bool showIcon: true
    property bool noOverlay: false
    property alias text: buttonText.text
    property bool chose: false
    property color contentColor: chose ? display.themeColor : display.contentColor
    flat: true

    width: buttonText.width + root.iconSize + root.fontSize * 1.5
    height: 32

    Image {
        id: image
        width: root.iconSize
        height: root.iconSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: root.fontSize * 0.3
        sourceSize: Qt.size(iconSize, iconSize)
        source: root.chose ? "qrc:/assets/check-checked.svg" : "qrc:/assets/check.svg"
        smooth: true
        antialiasing: true
        visible: root.noOverlay ? true : false
        rotation: root.chose ? 0 : 90

        Behavior on rotation {
            RotationAnimation {
                duration: 200
            }
        }
    }

    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        color: contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
        smooth: true
        antialiasing: true
        visible: root.noOverlay ? false : true
        rotation: root.chose ? 0 : 90

        Behavior on rotation {
            RotationAnimation {
                duration: 200
                easing.type: Easing.OutQuad
            }
        }
    }

    Text {
        id: buttonText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.showIcon ? image.right: undefined
        anchors.leftMargin: root.fontSize * 0.5
        anchors.horizontalCenter: root.showIcon ? undefined: parent.horizontalCenter
        color: contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        font.pixelSize: root.fontSize
    }
}
