import QtQuick
import QtQuick.Templates as T
import RxUI
import QtQuick.Window

T.Menu {
    id: control

    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    margins: 0
    overlap: 1
    padding: 1

    T.Overlay.modal: Rectangle {
        color: "transparent"
    }
    T.Overlay.modeless: Rectangle {
        color: "transparent"
    }
    background: Rectangle {
        border.color: Style.palette.mid
        border.width: 1 // FlyoutBorderThemeThickness
        color: Style.palette.toolTipBase
        implicitHeight: 38
        implicitWidth: 200
    }
    contentItem: ListView {
        clip: true
        currentIndex: control.currentIndex
        implicitHeight: contentHeight
        interactive: Window.window ? contentHeight + control.topPadding + control.bottomPadding > Window.window.height : false
        model: control.contentModel

        ScrollIndicator.vertical: ScrollIndicator {
        }
    }
    delegate: MenuItem {
    }
}