pragma Singleton
import QtQuick

QtObject {
    id: rxStyle

    property ColorGroup darkPalette: ColorGroup {
        alternateBase: "#202020"
        base: "transparent"
        brightText: "#ffffff"
        button: "#20ffffff"
        buttonText: "#f0f0f0"
        dark: "#30ffffff"
        highlight: "#403399FF"
        highlightedText: "#ffffff"
        light: "#10ffffff"
        link: rxStyle.primary
        linkVisited: Qt.darker(rxStyle.primary, 1.2)
        mid: "#28ffffff"
        midlight: "#18ffffff"
        placeholderText: "#40f0f0f0"
        shadow: "#000000"
        text: "#f0f0f0"
        toolTipBase: "#202020"
        toolTipText: "#f0f0f0"
        window: "#151515"
        windowText: "#f0f0f0"
    }
    property bool isDark: false
    property ColorGroup lightPalette: ColorGroup {
        alternateBase: "#10000000"
        base: "transparent"
        brightText: "#000000"
        button: "#04000000"
        buttonText: "#101010"
        dark: "#40000000"
        highlight: "#403399FF"
        highlightedText: "#000000"
        light: "#00000000"
        link: rxStyle.primary
        linkVisited: Qt.darker(rxStyle.primary, 1.2)
        mid: "#30000000"
        midlight: "#02000000"
        placeholderText: "#40000000"
        shadow: "#000000"
        text: "#101010"
        toolTipBase: "#eef4f9"
        toolTipText: "#101010"
        window: "#eef4f9"
        windowText: "#101010"
    }
    property ColorGroup palette: isDark ? darkPalette : lightPalette
    property color primary: "#3399FF"
    property color warning: "#FF9933"
}
