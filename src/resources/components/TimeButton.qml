import QtQuick 2.15
import QtGraphicalEffects 1.15

Rectangle {
    id: root

    property color normalColor: settings.colorStyle?"#30000000":"#20ffffff"
    property color hoverColor: settings.colorStyle?"#40000000":"#30ffffff"
    property color pressedColor: settings.colorStyle?"#50000000":"#40ffffff"
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
        color: settings.colorStyle? "#222222":"#dddddd"
        smooth: true
        antialiasing: true
        visible: !displayActive
    }

    Text {
        id: m_text;
        text: displayText;
        color: settings.colorStyle? "#222222":"#dddddd"
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
                displayActive: false
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                color: normalColor
                displayActive: true
            }
        },
        State {
            name: "Pressed"
            PropertyChanges {
                target: root
                color: pressedColor
                displayActive: false
            }
        }
    ]

    //define transmission for the states
    transitions: [
        Transition {
            from: "*"; to: "Hovering"
            ColorAnimation { duration: 70 }
            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: 1.0
                duration: 100
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "*"; to: "Pressed"
            ColorAnimation { duration: 70 }

            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: root.scaleWhenPressed? 0.97:1.0
                duration: 70
                easing.type: Easing.InOutQuad
            }
        },
        Transition {
            from: "*"; to: "Normal"
            ColorAnimation { duration: 100 }
            NumberAnimation {
                target: root
                property: "scale"
                from: root.scale
                to: 1.0
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
