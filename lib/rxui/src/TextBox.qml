import QtQuick
import QtQuick.Controls
import RxUI

Rectangle {
    id: root

    property color focusBorderColor: Style.primary
    property color focusColor: Style.palette.window
    property int fontSize: 16
    property color hoverBorderColor: Style.palette.dark
    property color hoverColor: "#30808080"
    property alias inputArea: inputTextBox
    property alias inputText: inputTextBox.text
    property color normalBorderColor: Style.palette.mid
    property color normalColor: "transparent"
    property string placeholder: ""

    signal enterPressed
    signal escPressed
    signal inputActive
    signal inputEdited(string input)
    signal inputFinished(string input)
    signal inputInactive
    signal inputRejected
    signal tabPressed

    function setInputFocus() {
        inputTextBox.forceActiveFocus();
    }

    border.width: 2
    clip: true
    implicitHeight: 36
    state: "Normal"

    states: [
        State {
            name: "Hovering"

            PropertyChanges {
                border.color: hoverBorderColor
                color: hoverColor
                target: root
            }
        },
        State {
            name: "Normal"

            PropertyChanges {
                border.color: normalBorderColor
                color: normalColor
                target: root
            }
        },
        State {
            name: "Focus"

            PropertyChanges {
                border.color: focusBorderColor
                color: focusColor
                target: root
            }
        }
    ]

    //define transmission for the states
    transitions: [
        Transition {
            from: "*"
            to: "Hovering"

            ColorAnimation {
                duration: 150
            }
        },
        Transition {
            from: "*"
            to: "Focus"

            ColorAnimation {
                duration: 150
            }
        },
        Transition {
            from: "*"
            to: "Normal"

            ColorAnimation {
                duration: 200
            }
        }
    ]

    TextInput {
        id: inputTextBox

        activeFocusOnPress: true
        anchors.left: parent.left
        anchors.leftMargin: 9
        anchors.right: parent.right
        anchors.rightMargin: 9
        anchors.verticalCenter: parent.verticalCenter
        // implicitWidth: root.width
        // canPaste: true
        clip: true
        color: Style.palette.text
        font.pixelSize: fontSize
        height: parent.height
        selectByMouse: true
        selectedTextColor: Style.palette.text
        selectionColor: Color.transparent(Style.primary, 0.4)
        text: inputText
        verticalAlignment: Text.AlignVCenter
        wrapMode: TextEdit.NoWrap

        cursorDelegate: Rectangle {
            id: cursorDelegate

            color: Color.transparent(Style.palette.buttonText, 0.6)
            width: 2

            Connections {
                function onActiveFocusChanged() {
                    if (target.activeFocus) {
                        cursorAnimation.start();
                    } else {
                        cursorAnimation.stop();
                    }
                }
                function onCursorPositionChanged() {
                    if (target.activeFocus) {
                        cursorAnimation.restart();
                    }
                }

                target: inputTextBox
            }
            SequentialAnimation {
                id: cursorAnimation

                loops: SequentialAnimation.Infinite
                running: false

                onStarted: {
                    cursorDelegate.visible = true;
                }
                onStopped: {
                    cursorDelegate.visible = false;
                }

                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCurve
                    from: 1
                    property: "opacity"
                    target: cursorDelegate
                    to: 0
                }
                PauseAnimation {
                    duration: 300
                }
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InCurve
                    from: 0
                    property: "opacity"
                    target: cursorDelegate
                    to: 1
                }
                PauseAnimation {
                    duration: 300
                }
            }
        }

        Keys.onPressed: function (event) {
            if (event.key === Qt.Key_Enter) {
                root.enterPressed();
                root.inputFinished(inputTextBox.text);
            } else if (event.key === Qt.Key_Esc)
                root.escPressed();
            else if (event.key === Qt.Key_Tab) {
                root.tabPressed();
                root.inputText = root.placeholder;
            }
        }
        onAccepted: {
            root.inputFinished(inputTextBox.text);
        }
        onActiveFocusChanged: {
            if (activeFocus) {
                root.state = "Focus";
                root.inputActive();
            } else {
                root.state = "Normal";
                root.inputInactive();
            }
        }
        onTextEdited: {
            root.inputEdited(inputTextBox.text);
        }

        MouseArea {
            acceptedButtons: Qt.RightButton
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor
            hoverEnabled: true

            onClicked: {
                contentMenu.popup();
            }

            Menu {
                id: contentMenu

                MenuItem {
                    icon.source: "qrc:/qt/qml/RxUI/assets/add-square-multiple.svg"
                    text: qsTr("Select All")

                    onTriggered: inputTextBox.selectAll()
                }
                MenuItem {
                    icon.source: "qrc:/qt/qml/RxUI/assets/cut-24.svg"
                    text: qsTr("Cut")

                    onTriggered: inputTextBox.cut()
                }
                MenuItem {
                    icon.source: "qrc:/qt/qml/RxUI/assets/copy.svg"
                    text: qsTr("Copy")

                    onTriggered: inputTextBox.copy()
                }
                MenuItem {
                    icon.source: "qrc:/qt/qml/RxUI/assets/clipboard-paste.svg"
                    text: qsTr("Paste")

                    onTriggered: inputTextBox.paste()
                }
            }
        }
    }
    Text {
        id: placeHolderText

        anchors.fill: parent
        anchors.leftMargin: 9
        anchors.rightMargin: 3 + root.height
        color: "#808080"
        font.pixelSize: fontSize
        horizontalAlignment: Text.AlignLeft
        text: placeholder
        verticalAlignment: Text.AlignVCenter
        visible: inputTextBox.text === "" ? true : false
    }
    MouseArea {
        id: hoverArea

        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        hoverEnabled: parent.enabled
        propagateComposedEvents: true

        onClicked: function (mouse) {
            mouse.accepted = false;
        }
        onEntered: {
            if (!inputTextBox.activeFocus)
                root.state = "Hovering";
        }
        onExited: {
            if (!inputTextBox.activeFocus)
                root.state = "Normal";
        }
        onPressed: function (mouse) {
            mouse.accepted = false;
        }
    }
}
