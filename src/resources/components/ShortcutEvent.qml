import QtQuick 2.15

Item {
    id: keyItem
    property var key
    property int isPressed: 0
    anchors.fill: parent
    focus: true

    signal keyClicked()

    Shortcut{
        sequence: key
        autoRepeat: false   //长按不触发
        context: Qt.ApplicationShortcut
        onActivated: {
            // console.log('clicked');
            keyItem.keyClicked();
        }
    }
}