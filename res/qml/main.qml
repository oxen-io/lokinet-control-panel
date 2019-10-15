import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.1

import StatFetcher 1.0
import LokinetApiClient 1.0
import "."

ApplicationWindow {
    id: window
    title: qsTr("Lokinet Control Panel")
    visible: false
    flags: Qt.FramelessWindowHint

    color: Style.panelSeparatorColor

    // crude way to dismiss main window when it loses focus.
    // this isn't going to work well on a "sloppy focus" window managers, but
    // probably works well on the most common OSes
    onActiveChanged: {
        if (!active) {
            window.visible = false;
        }
    }

    ControlPanel {
        id: controlPanel
    }

    // TODO:
    //  1) the "activated" signal doesn't seem to be firing in most cases, so we have a workaround
    //     that provides a "show" menu item.
    //  2) the provided menu shows on left-click; we would prefer to launch the control panel window
    //     on left click (and show the menu only on right-click).
    SystemTrayIcon {
        id: systray
        tooltip: qsTr("Loki Network")
        visible: true
        icon.source: "qrc:/res/images/icon.png"

        menu: Menu {
            enabled: false

            MenuItem {
                text: qsTr("Show")
                onTriggered: {
                    window.show();
                    window.raise();
                    window.requestActivate();
                }
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: {
                    Qt.quit();
                }
            }
        }

        onActivated: {
            // TODO: this isn't working for left or right click and works inconsistently with
            // middle click
            console.log(reason);
        }
    }

    StatFetcher {
        id: statFetcher
    }
    LokinetApiClient {
        id: apiClient
    }

}

