import QtQuick 2.15

Rectangle {
    id: root
    border.width: 2
    radius: 2
    property color normalColor: "#10808080"
    property color normalBorderColor: "#80808080"
    property color hoverColor: "#30808080"
    property color hoverBorderColor: "#A0808080"
    property color focusColor: "transparent"
    property color focusBorderColor: display.themeColor
    property string placeholder: ""
    property alias inputText: inputTextBox.text
    property int fontSize: 16
    property alias inputArea: inputTextBox
    clip: true

    signal inputEdited(string input)

    signal inputActive()

    signal inputInactive()

    signal inputFinished(string input)
    signal inputRejected()

    signal enterPressed()
    signal tabPressed()
    signal escPressed()

    function setInputFocus() {
        inputTextBox.forceActiveFocus();
    }

    state: "Normal"

    TextInput {
        id: inputTextBox
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            cursorShape: Qt.IBeamCursor
            hoverEnabled: true
            onClicked: {
                contentMenu.popup();
            }
            ContentMenu {
                id: contentMenu
                onItemClicked: {
                    switch (itemId) {
                    case 0: inputTextBox.selectAll();
                        break;
                    case 1: inputTextBox.copy();
                        break;
                    case 2: inputTextBox.cut();
                        break;
                    case 3: inputTextBox.paste();
                        break;
                    }
                }
                model: ListModel {
                    ListElement {
                        itemText: "Select All"
                        itemIcon: "qrc:/assets/select-all.svg"
                        itemId: 0
                    }
                    ListElement {
                        itemText: "Copy"
                        itemIcon: "qrc:/assets/copy.svg"
                        itemId: 1
                    }
                    ListElement {
                        itemText: "Cut"
                        itemIcon: "qrc:/assets/cut.svg"
                        itemId: 2
                    }
                    ListElement {
                        itemText: "Paste"
                        itemIcon: "qrc:/assets/clipboard.svg"
                        itemId: 3
                    }
                }
            }
        }

        text: inputText
        font.pixelSize: fontSize
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 9
        anchors.rightMargin: 3 + root.height
        activeFocusOnPress: true
        selectByMouse: true
        // implicitWidth: root.width
        // canPaste: true
        clip: true
        wrapMode: TextEdit.NoWrap
        color: display.contentColor
        selectedTextColor: display.contentColor
        selectionColor: "#603399ff"

        onTextEdited: {
            root.inputEdited(inputTextBox.text);
        }

         Keys.onPressed: {
            if (event.key === Qt.Key_Enter) {
                root.enterPressed();
                root.inputFinished(inputTextBox.text);
            }
            else if (event.key === Qt.Key_Esc)
                root.escPressed();
            else if (event.key === Qt.Key_Tab) {
                root.tabPressed();
                root.inputText = root.placeholder;
            }
        }

        onActiveFocusChanged: {
            if (activeFocus){
                root.state = "Focus";
                root.inputActive();
            }
            else{
                root.state = "Normal";
                root.inputInactive();
            }
        }

        onAccepted: {
            root.inputFinished(inputTextBox.text);
        }

        cursorDelegate: Rectangle {
            id: cursorDelegate

            width: 2

            color: display.themeColor

            Connections {
                target: inputTextBox

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
            }

            SequentialAnimation {
                id: cursorAnimation
                running: false
                loops: SequentialAnimation.Infinite

                onStarted: {
                    cursorDelegate.visible = true;
                }

                onStopped: {
                    cursorDelegate.visible = false;
                }

                NumberAnimation {
                    target: cursorDelegate
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 200
                    easing.type: Easing.OutCurve
                }

                PauseAnimation {
                    duration: 300
                }

                NumberAnimation {
                    target: cursorDelegate
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 200
                    easing.type: Easing.InCurve
                }

                PauseAnimation {
                    duration: 500
                }
            }
        }
    }

    Text {
        id: placeHolderText
        anchors.fill: parent
        font.pixelSize: fontSize
        visible: inputTextBox.text === "" ? true : false
        text: placeholder
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 9
        anchors.rightMargin: 3 + root.height
        color: "#808080"
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        propagateComposedEvents: true
        cursorShape: Qt.IBeamCursor
        hoverEnabled: parent.enabled

        onClicked: { mouse.accepted = false; }

        onPressed: { mouse.accepted = false; }

        onEntered: {
            if (!inputTextBox.activeFocus)
                root.state = "Hovering";
        }
        onExited: {
            if (!inputTextBox.activeFocus)
                root.state = "Normal";
        }
    }

    states: [
        State {
            name: "Hovering"
            PropertyChanges {
                target: root
                color: hoverColor
                border.color: hoverBorderColor
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: root
                color: normalColor
                border.color: normalBorderColor
            }
        },
        State {
            name: "Focus"
            PropertyChanges {
                target: root
                color: focusColor
                border.color: focusBorderColor
            }
        }
    ]

    //define transmission for the states
    transitions: [
        Transition {
            from: "*"; to: "Hovering"
            ColorAnimation {
                duration: 150 
            }
        },
        Transition {
            from: "*"; to: "Focus"
            ColorAnimation {
                duration: 150 
            }
        },
        Transition {
            from: "*"; to: "Normal"
            ColorAnimation {
                duration: 200
            }
        }
    ]

}
