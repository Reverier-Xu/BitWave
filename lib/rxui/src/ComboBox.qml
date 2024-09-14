import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Window
import RxUI

T.ComboBox {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, implicitContentHeight + topPadding + bottomPadding, implicitIndicatorHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, implicitContentWidth + leftPadding + rightPadding)
    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    padding: 8
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    HoverHandler {
        cursorShape: Qt.PointingHandCursor
    }

    background: Rectangle {
        border.color: control.enabled ? Style.palette.dark : Style.palette.mid
        border.width: control.flat ? 0 : 2 // ComboBoxBorderThemeThickness
        color: control.enabled ? (control.hovered ? Style.palette.mid : "transparent") : Style.palette.dark
        implicitHeight: 36
        implicitWidth: 120
        visible: !control.flat || control.pressed || control.hovered || control.visualFocus
    }

    contentItem: T.TextField {
        autoScroll: control.editable
        color: control.enabled ? Style.palette.buttonText : Style.palette.placeholderText
        enabled: control.editable
        inputMethodHints: control.inputMethodHints
        leftPadding: control.mirrored ? 1 : 4
        readOnly: control.down
        rightPadding: control.mirrored ? 4 : 1
        selectByMouse: control.selectTextByMouse
        selectedTextColor: Style.palette.highlightedText
        selectionColor: Style.palette.highlight
        text: control.editable ? control.editText : control.displayText
        validator: control.validator
        verticalAlignment: Text.AlignVCenter
    }

    delegate: ItemDelegate {
        required property int index

        hoverEnabled: control.hoverEnabled
        implicitHeight: 36
        text: control.model[index][control.textRole]
        width: ListView.view.width

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
        }

        background: Rectangle {
            color: hovered ? Style.palette.mid : "transparent"
            implicitHeight: 36
            implicitWidth: 200

            Behavior on color {
                ColorAnimation {
                    duration: 120
                }
            }
        }
    }

    indicator: ColorImage {
        color: control.enabled ? Style.palette.buttonText : Style.palette.mid
        source: "qrc:/qt/qml/RxUI/assets/chevron-down.svg"
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
    }

    popup: T.Popup {
        bottomMargin: 8
        height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin)
        implicitHeight: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin)
        topMargin: 8
        width: control.width
        y: control.height + 2

        background: Rectangle {
            anchors.fill: parent
            border.color: Style.palette.mid
            border.width: 1
            color: Style.palette.toolTipBase
        }

        contentItem: ListView {
            clip: true
            currentIndex: control.currentIndex
            implicitHeight: contentHeight
            interactive: Window.window ? contentHeight + control.topPadding + control.bottomPadding > Window.window.height : false
            model: control.delegateModel

            ScrollIndicator.vertical: ScrollIndicator {}
        }

        enter: Transition {
            NumberAnimation {
                duration: 120
                from: 0
                property: "opacity"
                to: 1
            }

            NumberAnimation {
                duration: 300
                easing.type: Easing.OutExpo
                from: control.popup.implicitHeight / 2
                property: "height"
                to: control.popup.implicitHeight
            }
        }

        exit: Transition {
            NumberAnimation {
                duration: 120
                from: 1
                property: "opacity"
                to: 0
            }

            NumberAnimation {
                duration: 300
                easing.type: Easing.OutExpo
                from: control.popup.implicitHeight
                property: "height"
                to: control.popup.implicitHeight / 2
            }
        }
    }
}
