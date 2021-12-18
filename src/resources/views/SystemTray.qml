import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1
import "qrc:/components"

SystemTrayIcon {
    id: systray
    visible: true
    icon.source: "qrc:/assets/logo-fill.svg"

    onActivated: {
        window.show()
        window.raise()
        window.requestActivate()
    }

    menu: Menu {
        id: menu
        visible: false

        MenuItem {
            icon.mask: true
            icon.name: "play"
            icon.source: player.isPlaying ? "qrc:/assets/pause.svg" : "qrc:/assets/play.svg"
            text: qsTr("Playing: ") + player.currentMediaTitle
            onTriggered: {
                window.show()
                window.raise()
                window.requestActivate()
            }
        }

        MenuItem {
            icon.source: player.isPlaying ? "qrc:/assets/pause.svg" : "qrc:/assets/play.svg"
            text: player.isPlaying ? qsTr("Pause") : qsTr("Play")
            onTriggered: player.isPlaying ? player.pause() : player.resume()
        }

        MenuItem {
            icon.name: "previous"
            icon.source: "qrc:/assets/previous.svg"
            text: qsTr("Previous")
            onTriggered: { queue.userPreviousRequested(); }
        }

        MenuItem {
            icon.source: "qrc:/assets/next.svg"
            text: qsTr("Next")
            onTriggered: { queue.userNextRequested(); }
        }
        
        MenuItem {
            icon.source: player.isMuted ? "qrc:/assets/volume-0.svg" : "qrc:/assets/volume-3.svg"
            text: player.isMuted ? qsTr("Unmute") : qsTr("Mute")
            onTriggered: {
                player.isMuted = !player.isMuted
            }
        }

        Menu {
            title: queue.playModeName
            
            MenuItem {
                icon.source: "qrc:/assets/play-repeat-all.svg"
                text: qsTr("Repeat All")
                onTriggered: { queue.repeatMode = 0; }
            }
            MenuItem {
                icon.source: "qrc:/assets/play-repeat-one.svg"
                text: qsTr("Repeat One")
                onTriggered: { queue.repeatMode = 1; }
            }
            MenuItem {
                icon.source: "qrc:/assets/play-random.svg"
                text: qsTr("Random")
                onTriggered: { queue.repeatMode = 2; }
            }
            MenuItem {
                icon.source: "qrc:/assets/play-order.svg"
                text: qsTr("In Order")
                onTriggered: { queue.repeatMode = 3; }
            }
            MenuItem {
                icon.source: "qrc:/assets/play-reverse.svg"
                text: qsTr("Reverse")
                onTriggered: { queue.repeatMode = 4; }
            }
        }
        
        MenuItem {
            icon.source: "qrc:/assets/close.svg"
            text: qsTr("Quit BitWave")
            onTriggered: Qt.quit()
        }
    }
}