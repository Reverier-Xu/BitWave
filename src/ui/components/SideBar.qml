import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: sideBar

    property bool expanded: true

    color: Color.transparent(Style.palette.windowText, 0.08)
    width: expanded ? 280 : 0

    Behavior on width {
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
        icon.color: "transparent"
        icon.height: 28
        icon.source: "qrc:/assets/logo-fill.svg"
        icon.width: 28
        hoverColor: "transparent"
        pressedColor: "transparent"
        text: "Bit Wave!"
    }
    ActiveTab {
        id: searchTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: logoTab.bottom
        anchors.topMargin: 6
        hoverColor: "transparent"
        pressedColor: "transparent"

        selected: router.currentRoute.startsWith("search")

        TextBox {
            id: searchBox

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            property string lastSearchText: ""

            placeholder: lastSearchText ? lastSearchText : qsTr("Hi, how are you?")
            onTabPressed: {
                inputText = lastSearchText
            }
            onInputFinished: {
                lastSearchText = inputText
                router.push(`search/${inputText}`)
                inputText = ""
            }
        }
    }
    ActiveTab {
        id: playerTab

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchTab.bottom
        anchors.topMargin: 12
        icon.source: "qrc:/qt/qml/RxUI/assets/play.svg"
        text: qsTr("Playing now")

        selected: router.currentRoute.startsWith("player")

        onClicked: {
            router.push("player")
        }
    }
    ListView {
        anchors.bottom: settingTab.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playerTab.bottom
        clip: true

        delegate: ActiveTab {
            anchors.topMargin: 4
            enabled: itemEnabled
            icon.source: itemIcon
            text: itemText
            width: ListView.view.width

            onClicked: {
                router.push(itemRoute)
            }
            selected: router.currentRoute.startsWith(itemRoute)
        }
        model: ListModel {
            ListElement {
                itemEnabled: true
                itemIcon: "qrc:/qt/qml/RxUI/assets/archive.svg"
                itemId: 0
                itemText: qsTr("Local Library")
                itemRoute: "libraries/local/"
            }
            ListElement {
                itemEnabled: true
                itemIcon: "qrc:/qt/qml/RxUI/assets/cloud.svg"
                itemId: 0
                itemText: qsTr("Online Media")
                itemRoute: "libraries/online/"
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

        selected: router.currentRoute.startsWith("settings")

        onClicked: {
            router.push("settings")
        }
    }
}
