import QtQuick 2.15
import QtGraphicalEffects 1.15

PushArea {
    id: root
    property string theTitle: qsTr("No Media")
    property string theAlbum: qsTr("Unknown Album")
    property string theArtist: qsTr("Unknown Artist")
    property bool isEndpoint: false
    property double theDuration
    property int theId
    property bool theStatus
    flat: true

    height: 36

    signal enterTriggered(int triggerId);
    signal removeTriggered(int triggerId);
    signal collectTriggered(int triggerId);

    Rectangle {
        anchors.fill: parent
        color: theId % 2 == 0 ? "#20808080" : "transparent"
    }

    Text {
        id: idLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 15
        color: "#808080"
        text: (root.theId + 1).toString().padStart(3, '0')
        font.pixelSize: 16
    }

    function getTimeString(displayTime) {
        let secs = Math.floor(displayTime);
        let minutes = Math.floor(secs/60);
        secs = secs % 60;
        return minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    Text {
        id: durationLabel
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        color: root.theStatus ? display.themeColor : "#808080"
        font.pixelSize: 16
        text: getTimeString(root.theDuration)
    }

    TextMetrics {
        id: titleMetrics
        elide: Text.ElideRight
        font.pixelSize: 16
        text: root.theTitle
        elideWidth: (root.width - durationLabel.width - idLabel.width) / 2 - 30
    }

    Text {
        id: titleLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: idLabel.right
        anchors.leftMargin: 15
        font.pixelSize: 16
        color: display.contentColor
        text: titleMetrics.elidedText
    }

    TextMetrics {
        id: infoMetrics
        elide: Text.ElideRight
        font.pixelSize: 16
        text: root.theArtist + " - " + root.theAlbum
        elideWidth: (root.width - durationLabel.width - idLabel.width) / 2 - 30
    }

    Text {
        id: infoLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: durationLabel.left
        anchors.rightMargin: 25
        font.pixelSize: 16
        color: display.contentColor
        text: infoMetrics.elidedText
    }

    ContentMenu {
        id: contentMenu
        onItemClicked: {
            switch (itemId) {
            case 0: root.enterTriggered(root.theId);
                break;
            case 1: root.collectTriggered(root.theId);
                break;
            case 2: root.removeTriggered(root.theId);
                break;
            }
        }
        model: ListModel {
            ListElement {
                itemText: root.isEndpoint ? qsTr("Play") : qsTr("Enter")
                itemIcon: "qrc:/assets/play.svg"
                itemId: 0
                itemEnabled: true
            }
            ListElement {
                itemText: qsTr("Add to Playlist")
                itemIcon: "qrc:/assets/add.svg"
                itemId: 1
                itemEnabled: root.isEndpoint
            }
            ListElement {
                itemText: qsTr("Remove")
                itemIcon: "qrc:/assets/delete.svg"
                itemId: 2
                itemEnabled: true
            }
        }
    }

    onRightClicked: {
        contentMenu.popup();
    }
}
