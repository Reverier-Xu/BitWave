import QtQuick 2.15
import "qrc:/components"

Rectangle {
    id: root
    color: "transparent"
    width: 280
    clip: true
    property bool expanded: display.sideBarExpanded
    state: expanded? "Expanded" : "Folded"

    Behavior on width {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }
    }

    TextLabel {
        id: titleButton
        height: 48
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        showIcon: true
        icon: "qrc:/assets/logo-fill.svg"
        iconSize: 28
        fontSize: 18
        bold: true
        noOverlay: true
        text: qsTr("Bit Wave!")
    }

    ActiveTab {
        id: globalSearchBox

        height: 36
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleButton.bottom
        flat: true
        border.color: "transparent"
        showIcon: false
        text: ""
        isTabActive: display.activeTabIndex === -3

        SearchBox {
            id: globalSearchBoxEdit
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: 32
            placeholder: qsTr("How are you today?")
            onInputFinished: {
                if (globalSearchBoxEdit.inputText.length > 0) {
                    globalSearchBoxEdit.placeholder = input;
                    globalSearchBoxEdit.inputText = "";
                    display.activeTabIndex = -3;
                    service.search(input);
                }
            }
        }
    }

    ActiveTab {
        id: playerPageButton
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: globalSearchBox.bottom
        anchors.topMargin: 12
        flat: true
        border.color: "transparent"
        showIcon: true
        icon: "qrc:/assets/play.svg"
        text: qsTr("Playing Now")
        isTabActive: display.activeTabIndex === -1
        onClicked: {
            display.activeTabIndex = -1
        }
    }

    ListView {
        id: libraryList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: playerPageButton.bottom
        anchors.topMargin: 3
        anchors.bottom: settingsButton.top
        anchors.bottomMargin: 3
        model: serviceListModel

        delegate: ActiveTab {
            width: ListView.view.width
            height: 36
            anchors.topMargin: 3
            flat: true
            border.color: "transparent"
            showIcon: true
            icon: serviceIcon
            text: serviceName
            isTabActive: index === display.activeTabIndex
            onClicked: {
                display.activeTabIndex = index
                service.visit(serviceName)
            }
        }
    }

    ActiveTab {
        id: settingsButton
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        flat: true
        border.color: "transparent"
        showIcon: true
        icon: "qrc:/assets/settings.svg"
        text: qsTr("Settings")
        isTabActive: display.activeTabIndex === -2
        onClicked: {
            display.activeTabIndex = -2
        }
    }

    states: [
        State {
            name: "Expanded"
            PropertyChanges {
                target: root
                width: 280
            }
        },
        State {
            name: "Folded"
            PropertyChanges {
                target: root
                width: 0
            }
        }
    ]
}
