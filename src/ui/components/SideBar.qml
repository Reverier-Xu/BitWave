import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: sideBar

    property bool expanded: true

    color: Color.transparent(Style.palette.windowText, 0.08)
    width: expanded ? 280 : 0

    Behavior on width  {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }
    }

    ActiveTab {
        id: logoTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        font.bold: true
        height: 48
        hoverColor: "transparent"
        icon.color: "transparent"
        icon.height: 28
        icon.source: "qrc:/assets/logo-fill.svg"
        icon.width: 28
        pressedColor: "transparent"
        text: "Bit Wave!"
    }
    TextBox {
        id: searchBox

        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: logoTab.bottom
        anchors.topMargin: 6
        placeholder: qsTr("Hi, how are you?")
    }
    ActiveTab {
        id: playerTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBox.bottom
        anchors.topMargin: 12
        icon.source: "qrc:/qt/qml/RxUI/assets/play.svg"
        text: qsTr("Playing now")
    }
    ListView {
        anchors.bottom: settingTab.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playerTab.bottom
        anchors.topMargin: 4

        delegate: ActiveTab {
            anchors.topMargin: 4
            enabled: itemEnabled
            icon.source: itemIcon
            text: itemText
            width: ListView.view.width
        }
        model: ListModel {
            ListElement {
                itemEnabled: true
                itemIcon: "qrc:/qt/qml/RxUI/assets/music-note-2.svg"
                itemId: 0
                itemText: qsTr("Local Library")
            }
        }
    }
    ActiveTab {
        id: settingTab

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.right: parent.right
        icon.source: "qrc:/qt/qml/RxUI/assets/settings.svg"
        text: qsTr("Settings")
    }
}
