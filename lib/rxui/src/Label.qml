import QtQuick
import QtQuick.Templates as T
import RxUI

T.Label {
    id: control

    color: Style.palette.windowText
    linkColor: Style.palette.link
    elide: Text.ElideRight

    property alias hovered: hoverHandler.hovered

    HoverHandler {
        id: hoverHandler
    }
}
