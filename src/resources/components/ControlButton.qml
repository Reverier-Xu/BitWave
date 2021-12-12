import QtQuick 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: root
    property color normalColor: display.themeColor
    property color hoverColor: display.colorStyle? Qt.lighter(normalColor, 1.2):Qt.darker(normalColor, 1.2)
    property color activeColor: display.alertColor
    property color pressedColor: display.colorStyle? Qt.lighter(hoverColor, 1.2):Qt.darker(hoverColor, 1.2)
    property bool scaleWhenPressed: true
    property bool displayActive: false
    property int fontSize: 16
    property string displayText: "00:00"
    property string icon: ""
    property int iconSize: 24
    property double displayTime: 0
    state: "Normal"

    border.color: normalColor
    border.width: 3
    color: display.colorStyle?"#e0cccccc":"#e0333333"
    radius: height / 2

    Behavior on border.color {
        ColorAnimation {
            duration: 200
        }
    }

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    Behavior on scale {
        NumberAnimation {
            duration: 200
            easing.type: Easing.OutExpo
        }
    }


    signal clicked();

    onDisplayTimeChanged: {
        let secs = Math.floor(displayTime);
        let minutes = Math.floor(secs/60);
        secs = secs % 60;
        displayText = minutes.toString().padStart(2, '0') + ":" + secs.toString().padStart(2, '0');
    }

    onDisplayActiveChanged: {
        if (displayActive)
            root.state = "Active";
        else
            root.state = "Normal";
    }

    Image {
        id: m_icon
        width: iconSize
        height: iconSize
        sourceSize: Qt.size(iconSize, iconSize)
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
                border.color: hoverColor
                scale: 1
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                border.color: normalColor
                scale: 1
            }
        },
        State {
            name: "Active"
            PropertyChanges {
                target: root
                border.color: activeColor
                scale: 1
            }
        },
        State {
            name: "Pressed"
            PropertyChanges {
                target: root
                border.color: pressedColor
                scale: root.scaleWhenPressed? 0.97 : 1
            }
        }
    ]
}
