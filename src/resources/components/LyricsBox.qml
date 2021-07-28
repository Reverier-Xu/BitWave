import QtQuick 2.15
import QtQml 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "transparent"
    clip: true
    property alias model: lyricList.model
    property alias currentIndex: lyricList.currentIndex

    ListView {
        id: lyricList
        anchors.fill: parent
        property bool isScrolling: false

        preferredHighlightBegin: height / 2 - 80
        preferredHighlightEnd: height / 2
        highlightRangeMode: ListView.StrictlyEnforceRange

        delegate: LyricContent {
            width: ListView.view.width
            lyricText: lyricContent
            isActive: index === lyricList.currentIndex
        }

        ScrollBar.vertical: UXScrollBar { }
    }
}
