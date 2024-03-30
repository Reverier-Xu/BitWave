import Qt.labs.platform
import QtQuick

SystemTrayIcon {
    id: systray

    icon.source: ui.flatSystemTray ? "qrc:/assets/logo-flat.svg" : "qrc:/assets/logo-fill.svg"
    visible: true
    onActivated: {
        window.show();
        window.raise();
        window.requestActivate();
    }

    menu: Menu {
        id: menu

        visible: false

        MenuItem {
            text: qsTr("Playing: ") + player.media.title
            onTriggered: {
                window.show();
                window.raise();
                window.requestActivate();
            }
        }

        MenuItem {
            text: player.playing ? qsTr("Pause") : qsTr("Play")
            onTriggered: player.playing ? player.pause() : player.resume()
        }

        MenuItem {
            text: qsTr("Previous")
            onTriggered: {
                queue.prev();
            }
        }

        MenuItem {
            text: qsTr("Next")
            onTriggered: {
                queue.next();
            }
        }

        MenuItem {
            text: player.muted ? qsTr("Unmute") : qsTr("Mute")
            onTriggered: {
                player.toggleMute();
            }
        }

        MenuItem {
            text: qsTr("Quit BitWave")
            onTriggered: app.requestQuit()
        }

    }

}
