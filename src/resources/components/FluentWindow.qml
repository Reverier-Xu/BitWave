import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

Window {
    id: root
    flags: Qt.Window | Qt.FramelessWindowHint
    visible: true
    color: "transparent"

    property bool style: true

    function toggleMaximized() {
        if (root.visibility === Window.Maximized) {
            root.showNormal()
        } else {
            root.showMaximized()
        }
    }

    MouseArea {
        // resize window mouse area
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: {
            if (root.state !== Window.Maximized) {
                const p = Qt.point(mouseX, mouseY)
                const b = 10
                // Increase the corner size slightly
                if (p.x < b && p.y < b)
                    return Qt.SizeFDiagCursor
                if (p.x >= width - b && p.y >= height - b)
                    return Qt.SizeFDiagCursor
                if (p.x >= width - b && p.y < b)
                    return Qt.SizeBDiagCursor
                if (p.x < b && p.y >= height - b)
                    return Qt.SizeBDiagCursor
                if (p.x < b || p.x >= width - b)
                    return Qt.SizeHorCursor
                if (p.y < b || p.y >= height - b)
                    return Qt.SizeVerCursor
            }
        }
        acceptedButtons: Qt.NoButton // don't handle actual events
    }

    DragHandler {
        id: resizeHandler
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
        onActiveChanged: {
            if (active && root.state !== Window.Maximized) {
                const p = resizeHandler.centroid.position
                // console.log(p);
                const b = 10
                // Increase the corner size slightly
                let e = 0
                // console.log(width - b);
                if (p.x < 2 * b) {
                    e |= Qt.LeftEdge
                }
                if (p.x >= width - 2 * b) {
                    e |= Qt.RightEdge
                }
                if (p.y < 2 * b) {
                    e |= Qt.TopEdge
                }
                if (p.y >= height - 2 * b) {
                    e |= Qt.BottomEdge
                }
                root.startSystemResize(e)
            }
        }
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2)
        setY(Screen.height / 2 - height / 2)
    }
}
