import QtQuick 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: display.colorStyle? "#ffffff" : "#000000"

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
}
