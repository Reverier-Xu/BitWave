import QtQuick
import QtQuick.Controls
import RxUI

Button {
    id: control

    property string iconSource: player.playing ? "qrc:/qt/qml/RxUI/assets/pause.svg" : "qrc:/qt/qml/RxUI/assets/play.svg"
    property bool showTime: false
    property int tempTime: 0

    display: showTime ? AbstractButton.TextOnly : AbstractButton.IconOnly
    icon.height: 20
    icon.source: iconSource
    icon.width: 20
    radius: width / 2
    text: {
        let secs = Math.floor(tempTime);
        let minutes = Math.floor(secs / 60);
        secs = secs % 60;
        return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    background: Rectangle {
        border.color: showTime ? Style.warning : Style.primary
        border.width: 3
        color: {
            if (pressed)
                return Style.palette.dark;
            else if (hovered)
                return Style.palette.mid;
            else
                return "transparent";
        }
        radius: width / 2

        Behavior on border.color {
            ColorAnimation {
                duration: 120
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 120
            }
        }
    }
}
