import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0

import ApiPoller 1.0
import LokinetApiClient 1.0
import PlatformDetails 1.0
import "."

ApplicationWindow {
    id: window
    title: qsTr("Lokinet Control Panel")
    visible: platformDetails.isDebug()
    flags: Qt.FramelessWindowHint

    color: Style.panelSeparatorColor

    // crude way to dismiss main window when it loses focus.
    // this isn't going to work well on a "sloppy focus" window managers, but
    // probably works well on the most common OSes
    onActiveChanged: {
        if (!active && !platformDetails.isDebug() && !platformDetails.isLinux()) {
            window.visible = false;
        }
    }

    ControlPanel {
        id: controlPanel
    }

    function display() {

        if (platformDetails.isLinux()) {
            // TODO: this allegedly returns 0,0 for x,y in some cases (e.g. some Linux WMs)
            //       so for now, we just hope the WM can do something intelligent for us
            //       (e.g. place the window near the mouse pointer)

        } else if (systray.geometry) { 

            // if Qt / our OS will at least give us systray geometry, use it to set the
            // position of our systray icon in its own screen coordinates
            const rect = systray.geometry;
            console.log("icon at: "+ rect.x + ", "+ rect.y);
            console.log("screen: "+ Screen.width +", "+ Screen.height);

            // attempt to determine what quadrant the systray is positioned in
            // and anchor the window to the opposite corner of the systray icon
            // TODO: this technique is flawed -- it often leaves the window partially obscured
            // by the taskbar (esp. when the taskbar is oriented horizontally)
            var right = false;
            if ((rect.x / Screen.width) >= 0.5) {
                right = true;
            }

            var bottom = false;
            if ((rect.y / Screen.height) >= 0.5) {
                bottom = true;
            }

            // on the right side, shift left by (window width)
            // on the left side, shift right by (systray icon width)
            // on the bottom side, shift up by (window height)
            // on the top side, shift down by systray icon height)
            var winX = (right ? (rect.x - window.width) : (rect.x + rect.width));
            var winY = (bottom ? (rect.y - window.height) : (rect.y + rect.height));

            window.x = winX;
            window.y = winY;

            console.log("updated window popup position: "+ window.x + ", "+ window.y);
        }

        window.show();
        window.raise();
        window.requestActivate();

    }

    SystemTrayIcon {
        id: systray
        tooltip: qsTr("Loki Network")
        visible: true
        iconSource: "qrc:/res/images/icon.png"

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
                text: qsTr("Hide")
                visible: (platformDetails.isDebug() || platformDetails.isLinux())
                onTriggered: {
                    window.visible = false;
                }
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: {
                    Qt.quit();
                }
            }

            MenuSeparator { }

            MenuItem {
                text: qsTr("Start lokinet")
                onTriggered: {
                    platformDetails.startLokinetProcess();
                }
            }
            MenuItem {
                text: qsTr("Kill lokinet")
                onTriggered: {
                    platformDetails.stopLokinetProcess();
                }
            }
            MenuItem {
                text: qsTr("Force-kill lokinet")
                onTriggered: {
                    platformDetails.forciblyStopLokinetProcess();
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

    ApiPoller {
        id: stateApiPoller
        Component.onCompleted: {
            stateApiPoller.setApiEndpoint("llarp.admin.dumpstate");
        }
    }
    ApiPoller {
        id: statusApiPoller
        Component.onCompleted: {
            statusApiPoller.setApiEndpoint("llarp.admin.status");
        }
    }
    LokinetApiClient {
        id: apiClient
    }
    PlatformDetails {
        id: platformDetails
    }

}

