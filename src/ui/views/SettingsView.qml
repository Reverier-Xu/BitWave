import Qt.labs.platform
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RxUI

Rectangle {
    id: view

    color: Style.palette.window

    ScrollView {
        id: container

        anchors.bottomMargin: 116
        anchors.fill: parent
        anchors.leftMargin: 32
        anchors.rightMargin: 32
        anchors.topMargin: 48

        Column {
            spacing: 6

            Label {
                id: title

                font.bold: true
                height: 36
                text: qsTr("Settings")
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                color: "transparent"
                height: 16
                width: 32
            }

            Label {
                id: themeTitle

                height: 36
                text: qsTr("Theme Settings")
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                color: "transparent"
                height: 36
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    height: 36
                    text: qsTr("Language")
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: languageComboBox

                    anchors.right: parent.right
                    currentIndex: {
                        if (ui.language === "en_US")
                            return 0;
                        else if (ui.language === "zh_CN")
                            return 1;
                    }
                    flat: true
                    model: [
                        {
                            "text": "English",
                            "value": "en_US"
                        },
                        {
                            "text": "简体中文",
                            "value": "zh_CN"
                        }
                    ]
                    textRole: "text"
                    valueRole: "value"
                    onActivated: {
                        ui.language = languageComboBox.currentValue;
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }

            Rectangle {
                color: "transparent"
                height: 36
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    height: 36
                    text: qsTr("Color Theme")
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: themeComboBox

                    anchors.right: parent.right
                    currentIndex: {
                        if (Style.isDark)
                            return 1;
                        else
                            return 0;
                    }
                    flat: true
                    model: [
                        {
                            "text": qsTr("Light"),
                            "value": false
                        },
                        {
                            "text": qsTr("Dark"),
                            "value": true
                        }
                    ]
                    textRole: "text"
                    valueRole: "value"
                    onActivated: {
                        Style.isDark = themeComboBox.currentValue;
                        ui.colorStyle = Style.isDark;
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }

            Rectangle {
                color: "transparent"
                height: 36
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    height: 36
                    text: qsTr("System tray theme")
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: trayThemeComboBox

                    anchors.right: parent.right
                    currentIndex: {
                        if (ui.flatSystemTray)
                            return 1;
                        else
                            return 0;
                    }
                    flat: true
                    model: [
                        {
                            "text": qsTr("Colorful"),
                            "value": false
                        },
                        {
                            "text": qsTr("Flat"),
                            "value": true
                        }
                    ]
                    textRole: "text"
                    valueRole: "value"
                    onActivated: {
                        ui.flatSystemTray = trayThemeComboBox.currentValue;
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }

            Rectangle {
                color: "transparent"
                height: 16
                width: 32
            }

            Label {
                id: libraryTitle

                height: 36
                text: qsTr("Library Settings")
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                color: "transparent"
                height: 36 * (library.folders.length + 1)
                width: container.width

                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.top: parent.top
                    height: 36
                    text: qsTr("Library Folders")
                    verticalAlignment: Text.AlignVCenter
                }

                Button {
                    anchors.right: scanButton.left
                    anchors.top: parent.top
                    height: 36
                    display: AbstractButton.TextBesideIcon
                    icon.source: "qrc:/qt/qml/RxUI/assets/branch.svg"
                    icon.color: library.recursiveScanning ? Style.primary : Style.palette.buttonText
                    icon.height: 16
                    icon.width: 16
                    text: qsTr("Scan folder recursively")
                    flat: true
                    onClicked: {
                        library.recursiveScanning = !library.recursiveScanning;
                    }
                }

                Button {
                    id: scanButton
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: 36
                    text: qsTr("Add")
                    icon.source: "qrc:/qt/qml/RxUI/assets/add.svg"
                    display: AbstractButton.TextBesideIcon
                    flat: true
                    onClicked: {
                        mediaFolderDialog.open();
                    }
                }

                ListView {
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.topMargin: 36
                    anchors.right: parent.right
                    anchors.left: parent.left
                    model: library.folders

                    delegate: Rectangle {
                        color: "transparent"
                        height: 36
                        width: ListView.view.width

                        RowLayout {
                            anchors.fill: parent
                            spacing: 0
                            anchors.leftMargin: 16

                            Button {
                                Layout.fillWidth: true
                                alignment: Qt.AlignVCenter | Qt.AlignLeft
                                display: AbstractButton.TextBesideIcon
                                icon.source: "qrc:/qt/qml/RxUI/assets/folder.svg"
                                icon.height: 16
                                icon.width: 16
                                text: modelData
                                flat: true
                            }

                            Button {
                                height: 36
                                width: 36
                                display: AbstractButton.IconOnly
                                icon.source: "qrc:/qt/qml/RxUI/assets/dismiss.svg"
                                icon.height: 16
                                icon.width: 16
                                flat: true
                                onClicked: {
                                    library.removeFolderPath(modelData);
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Style.palette.mid
                    height: 1
                }
            }
        }
    }

    FolderDialog {
        id: mediaFolderDialog

        title: qsTr("Open Media Folder")
        folder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        options: FolderDialog.ShowDirsOnly | FolderDialog.DontResolveSymlinks
        onAccepted: {
            library.addFolderPath(folder);
        }
    }
}
