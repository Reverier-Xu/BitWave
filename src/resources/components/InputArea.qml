import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    border.width: 2
    radius: 0
    property color normalColor: "#10808080"
    property color normalBorderColor: "#80808080"
    property color hoverColor: "#30808080"
    property color hoverBorderColor: "#A0808080"
    property color focusColor: "transparent"
    property color focusBorderColor: settings.themeColor
    property string placeholder: ""
    property string inputText: ""
    property int fontSize: 16
    property alias textObj: textBox

    signal inputEdited(string input)

    signal enterPressed()
    signal tabPressed()
    signal escPressed()

    state: "Normal"

    function setInputFocus() {
        textBox.forceActiveFocus();
    }

    ScrollBar {
        id: flickScrollbar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 6

    }

    Flickable {
        id: flick
        anchors.fill: parent
        anchors.margins: 15
        clip: true
        function ensureVisible(r) {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }
        contentHeight: textBox.paintedHeight
        contentWidth: textBox.paintedWidth
        ScrollBar.vertical: flickScrollbar
        TextEdit {
            id: textBox
            width: flick.width
            onPaintedHeightChanged: {
                if (paintedHeight < flick.height) {
                    height = flick.height;
                } else {
                    height = paintedHeight;
                }
            }

            onHeightChanged: {
                if (paintedHeight < flick.height) {
                    height = flick.height;
                } else {
                    height = paintedHeight;
                }
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Enter)
                    root.enterPressed()
                else if (event.key === Qt.Key_Esc)
                    root.escPressed()
                else if (event.key === Qt.Key_Tab)
                    root.tabPressed()
            }
    
            ContentMenu {
                id: contentMenu
                onItemClicked: {
                    switch (itemId) {
                    case 0: textBox.selectAll();
                        break;
                    case 1: textBox.copy();
                        break;
                    case 2:textBox.cut();
                        break;
                    case 3: textBox.paste();
                        break;
                    case 4: textBox.undo();
                        break;
                    }
                }
                model: ListModel {
                    ListElement {
                        itemText: "全选 (Ctrl+A)"
                        itemId: 0
                    }
                    ListElement {
                        itemText: "复制 (Ctrl+C)"
                        itemId: 1
                    }
                    ListElement {
                        itemText: "剪切 (Ctrl+X)"
                        itemId: 2
                    }
                    ListElement {
                        itemText: "粘贴 (Ctrl+V)"
                        itemId: 3
                    }
                    ListElement {
                        itemText: "撤销 (Ctrl+Z)"
                        itemId: 4
                    }
                }

            }

            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
            text: inputText
            font.pixelSize: fontSize
            activeFocusOnPress: true
            selectByMouse: true
            clip: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.PlainText
            color: settings.colorStyle ? "#000000" : "#ffffff"
            selectedTextColor: settings.colorStyle? "#222222" : "#dddddd"
            selectionColor: "#603399ff"

            onTextChanged: {
                root.inputEdited(textBox.text);
                root.inputText = textBox.text;
            }

            onActiveFocusChanged: {
                if (activeFocus){
                    root.state = "Focus";
                }
                else{
                    root.state = "Normal";
                }
            }

            cursorDelegate: Rectangle {
                id: cursorDelegate

                width: 2

                color: settings.themeColor

                Connections {
                    target: textBox

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
    }

    Text {
        id: placeHolderText
        anchors.centerIn: parent
        font.pixelSize: fontSize *1.6
        horizontalAlignment: Qt.AlignHCenter
        visible: textBox.text === "" ? true : false
        text: placeholder
        color: "#808080"
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        propagateComposedEvents: true
        cursorShape: Qt.IBeamCursor
        hoverEnabled: parent.enabled
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            mouse.accepted = false;
        }

        onReleased: {
            mouse.accepted = false;
        }

        onPressed: {
            textBox.forceActiveFocus();
            if (mouse.button === Qt.RightButton)
                contentMenu.popup();
            mouse.accepted = false;
        }

        onEntered: {
            if (!textBox.activeFocus)
                root.state = "Hovering";
        }
        onExited: {
            if (!textBox.activeFocus)
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

    transitions: [
        Transition {
            from: "*"; to: "Hovering"
            ColorAnimation { duration: 150 }
        },
        Transition {
            from: "*"; to: "Focus"
            ColorAnimation { duration: 150 }
        },
        Transition {
            from: "*"; to: "Normal"
            ColorAnimation { duration: 300 }
        }
    ]

}
