import QtQuick 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "#ffffff" : "#1b1b1b"

    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }

    StackLayout {
        id: stack
        anchors.fill: parent
        currentIndex: display.pageIndex
        PlayerPage { }
        PlaylistPage { }
        SearchPage { }
        SettingsPage { }
    }

    TextLabel {
        id: tipsLabel
        anchors.centerIn: parent
        radius: 4
        height: 64
        color: display.colorStyle ? "#c0ffffff" : "#c01b1b1b"
        iconSize: 32
        fontSize: 24
        opacity: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 300;
            }
        }

        Timer {
            id: fadeTimer
            interval: 800
            onTriggered: {
                tipsLabel.opacity = 0;
            }
        }

        Connections {
            target: display
            function onShowTips(icon, info) {
                tipsLabel.icon = icon;
                tipsLabel.text = info;
                tipsLabel.opacity = 1;
                fadeTimer.restart();
            }
        }
    }
}
