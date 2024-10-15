import Qt.labs.platform
import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: titleBar

    color: Qt.alpha(Style.palette.window, 0.95)
    height: ui.hideControls ? 0 : 36
    clip: true

    Button {
        id: closeButton

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        hoverColor: "#A0FF0000"
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/dismiss.svg"
        icon.width: 16
        pressedColor: "#A0FF0000"
        icon.color: hovered ? "#FFFFFF" : Style.palette.buttonText
        radius: 0
        width: 48
        onClicked: window.hide()
    }

    Button {
        id: maximizeButton

        anchors.right: closeButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: window.visibility === Window.Windowed ? "qrc:/qt/qml/RxUI/assets/maximize.svg" : "qrc:/qt/qml/RxUI/assets/restore.svg"
        icon.width: 16
        radius: 0
        width: 48
        onClicked: window.toggleMaximized()
    }

    Button {
        id: fullscreenButton

        anchors.right: maximizeButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: window.visibility === Window.FullScreen ? "qrc:/qt/qml/RxUI/assets/full-screen-minimize.svg" : "qrc:/qt/qml/RxUI/assets/full-screen-maximize.svg"
        icon.width: 16
        radius: 0
        width: 48
        onClicked: {
            if (window.visibility === Window.FullScreen)
                window.showNormal();
            else
                window.showFullScreen();
        }
    }

    Button {
        id: minimizeButton

        anchors.right: fullscreenButton.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/subtract.svg"
        icon.width: 16
        radius: 0
        width: 48
        onClicked: window.showMinimized()
    }

    Button {
        id: foldButton

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/navigation.svg"
        icon.width: 16
        radius: 0
        width: 48
        onClicked: ui.sideBarExpanded = !ui.sideBarExpanded
    }

    Button {
        id: backButton

        anchors.left: foldButton.right
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/arrow-left.svg"
        icon.width: 16
        radius: 0
        width: 48
        enabled: router.hasPrevious
        onClicked: router.pop()
    }

    Button {
        id: openFileButton

        anchors.left: backButton.right
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        flat: true
        icon.height: 16
        icon.source: "qrc:/qt/qml/RxUI/assets/open.svg"
        icon.width: 16
        radius: 0
        width: 48
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog

        title: qsTr("Open Media")
        nameFilters: ["All Support Files (*.mp3 *.m4a *.m4b *.m4p *.m4r *.m4v *.mp4 *.aac *.ape *.flac *.wma *.wv *.wav *.ogg *.ncm *.mp4 *.avi *.mkv *.flv *.mov *.wmv *.mpg *.mpeg *.m4v *.3gp *.3g2 *.mts *.m2ts *.ts *.m3u8 *.m3u *.mpd *.rm *.rmvb *.asf *.wma *.wmv *.flv *.f4v *.f4p *.f4a *.f4b *.vob *.webm)", "Music Files (*.mp3 *.m4a *.m4b *.m4p *.m4r *.m4v *.mp4 *.aac *.ape *.flac *.wma *.wv *.wav *.ogg)", "Netease Music Files (*.ncm)", "Video Files (*.mp4 *.avi *.mkv *.flv *.mov *.wmv *.mpg *.mpeg *.m4v *.3gp *.3g2 *.mts *.m2ts *.ts *.m3u8 *.m3u *.mpd *.rm *.rmvb *.asf *.wma *.wmv *.flv *.f4v *.f4p *.f4a *.f4b *.vob *.webm)"]
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        onAccepted: {
            let files_path = files.map(function (url) {
                return url.toString().replace("file:///", "/");
            });
            queue.addMediasByUrlsThenPlay(files_path);
        }
    }

    TapHandler {
        gesturePolicy: TapHandler.DragThreshold
        onTapped: {
            if (tapCount === 2)
                window.toggleMaximized();
        }
    }

    DragHandler {
        grabPermissions: TapHandler.DragThreshold
        onActiveChanged: {
            if (active)
                window.startSystemMove();
        }
    }

    HoverHandler {
        onHoveredChanged: {
            if (hovered)
                ui.blockHideControls();
            else
                ui.autoHideControls();
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutExpo
        }
    }
}
