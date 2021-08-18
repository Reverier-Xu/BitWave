import QtQuick 2.15

Rectangle {
    id: root
    color: display.colorStyle? "white" : "black"
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
}
