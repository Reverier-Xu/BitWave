import QtQuick 2.15
import QtGraphicalEffects 1.15

PushArea {
    id: root

    property color contentColor: settings.colorStyle? "#222222":"#dddddd"
    property int iconSize: 16
    property bool noOverlay: false
    property alias icon: iconImage.source

    Image {
        id: iconImage
        width: root.iconSize
        height: root.iconSize
        anchors.centerIn: parent
        smooth: true
        antialiasing: true
        visible: root.noOverlay?true:false
    }

    ColorOverlay {
        id: overlay
        anchors.fill: iconImage
        source: iconImage
        color: root.contentColor
        smooth: true
        antialiasing: true
        visible: root.noOverlay?false:true
    }
}
