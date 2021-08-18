import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "white" : "black"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
}
