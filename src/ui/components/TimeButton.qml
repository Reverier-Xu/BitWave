import QtQuick
import QtQuick.Controls
import RxUI

Button {
    id: control

    property int tempTime: 0

    display: hovered ? AbstractButton.IconOnly : AbstractButton.TextOnly
    implicitHeight: 48
    implicitWidth: 72
    icon.height: 16
    icon.width: 16
    radius: height / 2
    text: {
        let secs = Math.floor(tempTime);
        let minutes = Math.floor(secs / 60);
        secs = secs % 60;
        return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    background: Rectangle {
        border.color: Style.palette.mid
        border.width: 2
        color: {
            if (pressed)
                return Style.palette.dark;
            else if (hovered)
                return Style.palette.mid;
            else
                return "transparent";
        }
        radius: height / 2

        Behavior on color  {
            ColorAnimation {
                duration: 120
            }
        }
        Behavior on border.color  {
            ColorAnimation {
                duration: 120
            }
        }
    }
}
