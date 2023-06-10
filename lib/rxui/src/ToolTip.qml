import QtQuick
import QtQuick.Templates as T
import RxUI

T.ToolTip {
    id: control

    bottomPadding: padding - 1
    closePolicy: T.Popup.CloseOnEscape | T.Popup.CloseOnPressOutsideParent | T.Popup.CloseOnReleaseOutsideParent
    delay: Qt.styleHints.mousePressAndHoldInterval
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    margins: 8
    padding: 8
    x: parent ? (parent.width - implicitWidth) / 2 : 0
    y: implicitHeight

    background: Rectangle {
        border.color: Style.palette.mid
        border.width: 1
        color: Style.palette.toolTipBase
    }
    contentItem: Text {
        color: Style.palette.toolTipText
        font: control.font
        opacity: enabled ? 1.0 : 0.2
        text: control.text
        wrapMode: Text.Wrap
    }
}
