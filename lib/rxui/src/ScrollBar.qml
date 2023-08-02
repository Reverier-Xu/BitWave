import QtQuick
import QtQuick.Templates as T
import RxUI

T.ScrollBar {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    minimumSize: 0.1
    visible: control.policy !== T.ScrollBar.AlwaysOff

    background: Rectangle {
        color: "transparent"
        implicitHeight: 8
        implicitWidth: 8
        opacity: 0.0
        visible: control.size < 1.0
    }
    contentItem: Rectangle {
        color: "transparent"
        implicitHeight: 8
        implicitWidth: 8
        opacity: 0.0
        Rectangle {
            anchors.fill: parent
            anchors.margins: 2
            color: control.pressed ? Style.palette.dark : enabled && control.interactive && control.hovered ? Style.palette.mid : Style.palette.button
            radius: width > height ? height / 2 : width / 2
        }
    }
    states: [
        State {
            name: "active"
            when: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0)
        }
    ]
    transitions: [
        Transition {
            to: "active"

            NumberAnimation {
                property: "opacity"
                targets: [control.contentItem, control.background]
                to: 1.0
            }
        },
        Transition {
            from: "active"

            SequentialAnimation {
                PropertyAction {
                    property: "opacity"
                    targets: [control.contentItem, control.background]
                    value: 1.0
                }
                PauseAnimation {
                    duration: 3000
                }
                NumberAnimation {
                    property: "opacity"
                    targets: [control.contentItem, control.background]
                    to: 0.0
                }
            }
        }
    ]
}