import QtQuick
import QtQuick.Controls
import RxUI

Button {
    id: control

    alignment: Qt.AlignVCenter | Qt.AlignLeft
    display: AbstractButton.TextBesideIcon
    flat: true
    icon.height: 16
    icon.width: 16
    leftPadding: 12
    rightPadding: 12
    spacing: 12
    property bool selected: false

    Rectangle {
        id: indicator

        visible: selected
        anchors.verticalCenter: parent.verticalCenter
        radius: control.hovered ? 0 : width / 2
        width: 3
        x: control.hovered ? 0 : 6
        height: control.hovered ? parent.height : parent.height - 8
        color: Style.primary

        Behavior on x {
            NumberAnimation {
                duration: 300
                easing.type: Easing.OutExpo
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 120
            }
        }
    }
}
