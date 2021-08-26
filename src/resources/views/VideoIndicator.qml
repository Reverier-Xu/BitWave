import QtQuick 2.15
import "qrc:/components"

Item {
    id: root
    enabled: display.showVideoTime
    opacity: display.showVideoTime && player.currentMediaIsVideo ? 1 : 0

    width: systemTimeLabel.width
    height: 64

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    TextLabel {
        id: systemTimeLabel
        showIcon: false
        bold: true
        contentColor: "#B0FFFFFF"
        anchors.centerIn: parent
        fontSize: 32
        height: 64
        text: currentDateTime()

        function currentDateTime() {
            return Qt.formatDateTime(new Date(), "hh:mm");
        }
    }

    Timer {
        id: timer
        interval: 3
        repeat: true
        running: display.showVideoTime && player.currentMediaIsVideo
        onTriggered: {
            systemTimeLabel.text = systemTimeLabel.currentDateTime();
        }
    }
}
