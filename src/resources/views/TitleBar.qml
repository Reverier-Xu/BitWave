import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1
import "../components"

Rectangle {
    id: root
    color: display.colorStyle ? "#a0ffffff":"#d0000000"
    opacity: display.mouseIsActive ? 1 : 0
    Behavior on opacity {
        NumberAnimation {
            duration: 100
        }
    }
    Behavior on color {
        ColorAnimation {
            duration: 280
        }
    }
    height: 32

    IconButton {
        id: closeButton
        icon: "qrc:/assets/close.svg"
        iconSize: 16
        pressedColor: "#ccee0000"
        hoverColor: "#aaff0000"
        border.color: "transparent"
        flat: true
        anchors.right: parent.right
        anchors.top: parent.top
        width: 54
        height: 32
        onClicked: {
            window.hide();
        }
    }

    IconButton {
        id: maximizeButton
        icon: window.visibility === Window.Windowed?"qrc:/assets/maximize.svg":"qrc:/assets/restore.svg"
        iconSize: 16
        flat: true
        anchors.right: closeButton.left
        anchors.top: parent.top
        border.color: "transparent"
        width: 54
        height: 32
        onClicked: {
            if (window.visibility === Window.Maximized || window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showMaximized();
        }
    }

    IconButton {
        id: fullscreenButton
        width: 54
        height: 32
        iconSize: 16
        flat: true
        anchors.right: maximizeButton.left
        anchors.top: parent.top
        border.color: "transparent"
        icon: window.visibility === Window.FullScreen ? "qrc:/assets/exit-fullscreen.svg" : "qrc:/assets/enter-fullscreen.svg"
        onClicked: {
            if (window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showFullScreen();
        }
    }

    IconButton {
        id: minimizeButton
        icon: "qrc:/assets/minimize.svg"
        iconSize: 16
        flat: true
        anchors.right: fullscreenButton.left
        anchors.top: parent.top
        border.color: "transparent"
        width: 54
        height: 32
        onClicked: {
            window.showMinimized();
        }
    }

    // IconButton {
    //     id: colorStyleButton
    //     icon: display.colorStyle ? "qrc:/assets/sun.svg" : "qrc:/assets/moon.svg"
    //     iconSize: 16
    //     flat: true
    //     anchors.right: minimizeButton.left
    //     anchors.top: parent.top
    //     border.color: "transparent"
    //     width: 54
    //     height: 32
    //     onClicked: {
    //         display.colorStyle = !display.colorStyle
    //     }
    // }

    IconButton {
        id: titleButton
        icon: "qrc:/assets/navigation.svg"
        anchors.left: parent.left
        anchors.top: parent.top
        height: 32
        width: 54
        flat: true
        border.color: "transparent"
        onClicked: {
            display.sideBarExpanded = !display.sideBarExpanded;
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Open Media")
        nameFilters: [
            "All Support Files (*.mp3 *.m4a *.m4b *.m4p *.m4r *.m4v *.mp4 *.aac *.ape *.flac *.wma *.wv *.wav *.ogg *.ncm *.mp4 *.avi *.mkv *.flv *.mov *.wmv *.mpg *.mpeg *.m4v *.3gp *.3g2 *.mts *.m2ts *.ts *.m3u8 *.m3u *.mpd *.rm *.rmvb *.asf *.wma *.wmv *.flv *.f4v *.f4p *.f4a *.f4b *.vob *.webm)",
            "Music Files (*.mp3 *.m4a *.m4b *.m4p *.m4r *.m4v *.mp4 *.aac *.ape *.flac *.wma *.wv *.wav *.ogg)",
            "Netease Music Files (*.ncm)",
            "Video Files (*.mp4 *.avi *.mkv *.flv *.mov *.wmv *.mpg *.mpeg *.m4v *.3gp *.3g2 *.mts *.m2ts *.ts *.m3u8 *.m3u *.mpd *.rm *.rmvb *.asf *.wma *.wmv *.flv *.f4v *.f4p *.f4a *.f4b *.vob *.webm)"
        ]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            queue.addExternMedia(files[0]);
        }
    }

    IconButton {
        id: openExternMediaButton
        icon: "qrc:/assets/open.svg"
        anchors.left: titleButton.right
        anchors.top: parent.top
        height: 32
        width: 54
        flat: true
        border.color: "transparent"
        onClicked: fileDialog.open()
    }

    TapHandler {
        onTapped: if (tapCount === 2) toggleMaximized();
        gesturePolicy: TapHandler.DragThreshold
    }

    DragHandler {
        grabPermissions: TapHandler.DragThreshold
        onActiveChanged: {
            if (active) {
                window.startSystemMove();
            }
        }
    }

    HoverHandler {
        onHoveredChanged: {
            if (hovered) {
                // console.log("hovered");
                display.blockDelayedHide();
            } else {
                // console.log("unhovered");
                display.delayedHide();
            }
        }
    }
}
