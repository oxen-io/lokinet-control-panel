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

    function display() {
        // attempt to position at the system tray icon
        // TODO: need to be more intelligent about this:
        // 1) not all platforms provide proper geometry (return 0,0)
        // 2) task bars can be in many different orientations
        const rect = systray.geometry;
        console.log("icon at: "+ rect.x + ", "+ rect.y);
        window.x = rect.x - (window.width - 10);
        window.y = rect.y - (window.height - 10);

        window.show();
        window.raise();
        window.requestActivate();

    }

    SystemTrayIcon {
        id: systray
        tooltip: qsTr("Loki Network")
        visible: true
        icon.source: "qrc:/res/images/icon.png"

        menu: Menu {
            id: systrayMenu
            enabled: true

            MenuItem {
                text: qsTr("Show")
                onTriggered: {
                    window.display();
                }
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: {
                    Qt.quit();
                }
            }
        }

        // under some Linux window managers, intercepting left click (or even any click in some
        // cases) is not possible. in these cases, the context menu seems to be reliably shown,
        // so interaction with the system tray icon will have to be through the context menu in
        // these cases.
        //
        // this leaves our behavior as:
        // - left/double click: show main window (if we get these events)
        // - right click: show context menu (if we get these events)
        // - fallback: hope the OS/WM at least gives us the context menu automatically
        onActivated: {
            switch(reason) {

                // right click
                case SystemTrayIcon.Context:
                    systrayMenu.open();
                    break;

                // left click
                case SystemTrayIcon.Trigger:
                case SystemTrayIcon.DoubleClick:
                    window.display();
                    break;

                default:
                    console.log("systray icon 'activated', but ignoring. reason: "+ reason);
            }

        }
    }

    StatFetcher {
        id: statFetcher
    }
    LokinetApiClient {
        id: apiClient
    }

}

