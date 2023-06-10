import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl
import RxUI

T.Button {
    id: control

    property int alignment: Qt.AlignVCenter | Qt.AlignHCenter
    property color hoverColor: Style.palette.mid
    property color normalColor: Style.palette.button
    property color pressedColor: Style.palette.dark
    property int radius: 0

    display: AbstractButton.TextOnly
    flat: false
    icon.color: Style.palette.buttonText
    icon.height: 20
    icon.width: 20
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    padding: 6
    spacing: 6

    background: Rectangle {
        border.color: Style.palette.mid
        border.width: flat ? 0 : 1
        color: {
            if (pressed)
                return control.pressedColor;
            else if (hovered)
                return control.hoverColor;
            else if (flat)
                return "transparent";
            else
                return control.normalColor;
        }
        implicitHeight: 32
        implicitWidth: 32
        radius: control.radius

        Behavior on color  {
            ColorAnimation {
                duration: 120
            }
        }
    }
    contentItem: IconLabel {
        alignment: control.alignment
        color: Color.transparent(Style.palette.buttonText, enabled ? 1.0 : 0.2)
        display: control.display
        font: control.font
        icon: control.icon
        mirrored: control.mirrored
        spacing: control.spacing
        text: control.text
    }

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }
}
