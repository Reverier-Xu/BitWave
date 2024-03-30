import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import RxUI

T.MenuItem {
    id: control

    icon.color: !enabled ? Color.transparent(Style.palette.text, 0.6) : Style.palette.text
    icon.height: 16
    icon.width: 16
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: 12
    padding: 6
    rightPadding: 12
    spacing: 12

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    arrow: ColorImage {
        color: !enabled ? Color.transparent(Style.palette.text, 0.4) : Style.palette.text
        mirror: control.mirrored
        source: "qrc:/qt/qml/RxUI/assets/chevron-right.svg"
        visible: control.subMenu
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
    }

    background: Rectangle {
        color: !control.enabled ? Color.transparent(Style.palette.text, 0.2) : control.down ? Style.palette.dark : control.highlighted ? Style.palette.mid : "transparent"
        implicitHeight: 36
        implicitWidth: 200

        Rectangle {
            color: Style.palette.highlight
            height: parent.height - 2
            opacity: 0.5
            visible: control.visualFocus
            width: parent.width - 2
            x: 1
            y: 1
        }

        Behavior on color {
            ColorAnimation {
                duration: 120
            }

        }

    }

    contentItem: IconLabel {
        readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
        readonly property real indicatorPadding: control.checkable && control.indicator ? control.indicator.width + control.spacing : 0

        alignment: Qt.AlignLeft
        color: !control.enabled ? Color.transparent(Style.palette.text, 0.6) : Style.palette.text
        display: control.display
        font: control.font
        icon: control.icon
        leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
        mirrored: control.mirrored
        rightPadding: control.mirrored ? indicatorPadding : arrowPadding
        spacing: control.spacing
        text: control.text
    }

    indicator: ColorImage {
        color: !control.enabled ? Color.transparent(Style.palette.text, 0.4) : control.down ? Style.palette.text : Style.palette.highlightText
        source: !control.checkable ? "" : "qrc:/qt/qml/RxUI/assets/checkmark.svg"
        visible: control.checked
        x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
    }

}
