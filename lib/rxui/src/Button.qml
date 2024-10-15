import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import RxUI

T.Button {
    id: control

    property int alignment: Qt.AlignVCenter | Qt.AlignHCenter
    property color hoverColor: Style.palette.mid
    property color normalColor: Style.palette.button
    property color pressedColor: Style.palette.dark
    property int radius: 0
    property color textColor: Qt.alpha(Style.palette.buttonText, enabled ? 1 : 0.2)

    display: AbstractButton.TextOnly
    flat: false
    icon.color: Style.palette.buttonText
    icon.height: 20
    icon.width: 20
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        border.color: Style.palette.mid
        border.width: flat ? 0 : 1
        color: {
            if (!enabled)
                return Style.palette.midlight;
            if (pressed)
                return control.pressedColor;
            else if (hovered)
                return control.hoverColor;
            else if (flat)
                return "transparent";
            else
                return control.normalColor;
        }
        implicitHeight: 36
        implicitWidth: 36
        radius: control.radius

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }
    }

    contentItem: IconLabel {
        alignment: control.alignment
        color: control.textColor
        display: control.display
        font: control.font
        icon: control.icon
        mirrored: control.mirrored
        spacing: control.spacing
        text: control.text
        opacity: control.enabled ? 1 : 0.6
    }
}
