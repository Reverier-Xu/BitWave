import QtQuick 2.15
import QtGraphicalEffects 1.15

PushArea {
    id: root
    property string theTitle: "No Media"
    property string theAlbum: "Unknown Album"
    property string theArtist: "Unknown Artist"
    property double theDuration
    property int theId
    property bool theStatus
    flat: true

    height: 36

    signal playTriggered(int triggerId);
    signal removeTriggered(int triggerId);
    signal clearTriggered();

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
        opacity: root.theStatus ? 0 : 1
        text: (root.theId + 1).toString().padStart(3, '0')
        font.pixelSize: 16
    }

    Image {
        id: image
        width: 16
        height: 16
        anchors.verticalCenter: parent.verticalCenter
        anchors.centerIn: idLabel
        sourceSize: Qt.size(16, 16)
        smooth: true
        antialiasing: true
        visible: false
        source: "qrc:/assets/current.svg"
    }

    ColorOverlay {
        id: overlay
        anchors.fill: image
        source: image
        color: display.themeColor
        smooth: true
        antialiasing: true
        visible: root.theStatus
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
        elideWidth: titleLabel.width
    }

    Text {
        id: titleLabel
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: idLabel.right
        anchors.right: durationLabel.left
        anchors.leftMargin: 15
        anchors.rightMargin: 25
        font.pixelSize: 16
        color: display.contentColor
        text: titleMetrics.elidedText
    }

    ContentMenu {
        id: contentMenu
        onItemClicked: {
            switch (itemId) {
            case 0: root.playTriggered(root.theId);
                break;
            case 1: root.removeTriggered(root.theId);
                break;
            case 2: root.clearTriggered();
                break;
            }
        }
        model: ListModel {
            ListElement {
                itemText: "Play"
                itemIcon: "qrc:/assets/play.svg"
                itemId: 0
            }
            ListElement {
                itemText: "Remove"
                itemIcon: "qrc:/assets/delete.svg"
                itemId: 1
            }
            ListElement {
                itemText: "Clear Queue"
                itemIcon: "qrc:/assets/close.svg"
                itemId: 2
            }
        }
    }

    onRightClicked: {
        contentMenu.popup();
    }
}
