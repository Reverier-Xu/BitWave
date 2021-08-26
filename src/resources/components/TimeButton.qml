import QtQuick 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: root

    property color normalColor: display.colorStyle?"#e0cccccc":"#e0333333"
    property color hoverColor: display.colorStyle?"#e0dddddd":"#e0444444"
    property color pressedColor: display.colorStyle?"#e0eeeeee":"#e0555555"
    property bool scaleWhenPressed: true
    property bool displayActive: true
    property int fontSize: 16
    property string displayText: "00:00"
    property string icon: ""
    property int iconSize: 16
    property double displayTime: 0

    radius: height/2
    color: normalColor
    border.width: 2
    border.color: "#40808080"

    signal clicked()

    onDisplayTimeChanged: {
        let secs = Math.floor(displayTime);
        let minutes = Math.floor(secs/60);
        secs = secs % 60;
        displayText = minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    Image {
        id: m_icon
        width: iconSize
        height: iconSize
        anchors.centerIn: parent
        source: icon

        smooth: true;
        visible: false;
    }

    ColorOverlay{
        anchors.fill: m_icon
        source: m_icon
        color: display.contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        smooth: true
        antialiasing: true
        visible: !displayActive
    }

    Text {
        id: m_text;
        text: displayText;
        color: display.contentColor
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }
        font.pixelSize: root.fontSize;
        anchors.centerIn: root
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        visible: displayActive
    }

    MouseArea {
        id: m_mouseArea;
        anchors.fill: parent;
        hoverEnabled: parent.enabled;
        onEntered: {
            if (!pressed)
                parent.state="Hovering";
        }
        onExited: {
            if (!pressed)
                parent.state = "Normal";
        }
        onClicked: {
            parent.clicked();
            root.focus = true;
        }
        onPressed: {
            parent.state="Pressed";
        }
        onReleased: {
            if (containsMouse)
                parent.state="Hovering";
            else
                parent.state = "Normal";
        }
    }

    states: [
        State {
            name: "Hovering"
            PropertyChanges {
                target: root
                color: hoverColor
                scale: 1
                displayActive: false
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                color: normalColor
                scale: 1
                displayActive: true
            }
        },
        State {
            name: "Pressed"
            PropertyChanges {
                target: root
                color: pressedColor
                scale: 0.97
                displayActive: false
            }
        }
    ]

    Behavior on color {
        ColorAnimation {
            duration: 100
        }
    }
    Behavior on scale {
        NumberAnimation {
            duration: 200
        }
    }
}
