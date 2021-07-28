import QtQuick 2.15
import QtQuick.Layouts 1.15
import "qrc:/components"

Rectangle {
    id: root
    color: settings.colorStyle? "#ffffff" : "#000000"

    StackLayout {
        id: stack
        anchors.fill: parent
        currentIndex: 0
        PlayerPage { }
        PlaylistPage { }
        SearchPage { }
        SettingsPage { }
    }
}
