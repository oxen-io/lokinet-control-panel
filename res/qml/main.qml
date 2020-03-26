import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import Qt.labs.platform 1.0

import ApiPoller 1.0
import LokinetApiClient 1.0
import PlatformDetails 1.0
import "."

ApplicationWindow {
    id: window
    title: qsTr("Lokinet Control Panel")
    visible: nohide
    flags: nohide ? Qt.Window : Qt.FramelessWindowHint
    maximumHeight: minimumHeight
    maximumWidth: minimumWidth

    color: Style.panelSeparatorColor

    // crude way to dismiss main window when it loses focus.
    // this isn't going to work well on a "sloppy focus" window managers, but
    // probably works well on the most common OSes
    onActiveChanged: {
        if (!active && !nohide) {
            window.visible = false;
        }
    }

    onClosing: {
        if (notray) {
            window.exitApp();
        }
    }

    ControlPanel {
        id: controlPanel
    }

    BootstrapDownloadDialog {
        id: bootstrapDialog
        visible: false
    }

    function exitApp() {
        if (! platformDetails.isDebug()) {
            platformDetails.stopLokinetIfWeStartedIt();
        }
        Qt.quit();
    }

    function display() {

        var rect = platformDetails.getAbsoluteCursorPosition();
        console.log("mouse cursor at: "+ rect.x +", "+ rect.y);

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

        console.log("right: "+ right);
        console.log("bottom: "+ bottom);

        console.log("window size: "+ window.width +"x"+ window.height);

        // on the right side, shift left by (window width)
        // on the left side, shift right by (systray icon width)
        // on the bottom side, shift up by (window height)
        // on the top side, shift down by systray icon height)
        var winX = (right ? (rect.x - window.width) : rect.x);
        var winY = (bottom ? (rect.y - window.height) : rect.y);

        window.x = winX;
        window.y = winY;

        console.log("updated window popup position: "+ window.x + ", "+ window.y);

        window.show();
        window.raise();
        window.requestActivate();

    }

    SystemTrayIcon {
        id: systray
        tooltip: qsTr("Loki Network")
        visible: !notray
        iconSource: "qrc:/res/images/icon.svg"

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
                    window.exitApp();
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
                text: qsTr("Stop lokinet")
                onTriggered: {
                    platformDetails.managedStopLokinetProcess();
                }
            }

            MenuSeparator { }

            MenuItem {
                visible: !isSystemd
                text: qsTr("Bootstrap Client from Web")
                onTriggered: {
                    bootstrapDialog.visible = true;
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

                    // Qt on MacOS only gives us one event to work with, namely
                    // onActivated() where action == Trigger. It will also try to
                    // show the context menu at the same time.
                    // We don't want both, and we only get one, so we let Qt show
                    // the context menu.
                    if (! platformDetails.isMacOS()) {
                        window.display();
                    }
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

