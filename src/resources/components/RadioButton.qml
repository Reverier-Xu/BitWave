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
    property color contentColor: display.contentColor
    property bool chose: false
    property bool bold: chose

    Rectangle {
        id: roundRect
        width: root.iconSize
        radius: width / 2
        height: root.iconSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: root.fontSize * 0.7
        color: "transparent"
        border.width: 2
        border.color: root.chose ? display.themeColor : "#80808080"
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        Rectangle {
            id: pointRect
            width: root.iconSize - 8
            height: root.iconSize - 8
            radius: width / 2
            anchors.centerIn: roundRect
            color: root.chose ? display.themeColor : "transparent"
            Behavior on color {
                ColorAnimation {
                    duration: 200
                }
            }
        }
    }

    Image {
        id: image
        width: root.iconSize
        height: root.iconSize
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: roundRect.right
        anchors.leftMargin: root.fontSize * 0.7
        smooth: true
        antialiasing: true
        visible: root.noOverlay?true:false
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
        visible: root.noOverlay?false:true
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
