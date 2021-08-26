import QtQuick 2.15
import QtGraphicalEffects 1.15

CircleSliderArea {
    id: root

    property color contentColor: display.contentColor
    property int iconSize: 16
    property bool noOverlay: false
    property alias icon: iconImage.source
    property int rotation: 0

    Image {
        id: iconImage
        width: root.iconSize
        height: root.iconSize
        anchors.centerIn: parent
        smooth: true
        antialiasing: true
        visible: root.noOverlay ? true : false
    }

    ColorOverlay {
        id: overlay
        anchors.fill: iconImage
        source: iconImage
        rotation: root.rotation
        color: root.contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        smooth: true
        antialiasing: true
        visible: root.noOverlay ? false : true
    }
}
