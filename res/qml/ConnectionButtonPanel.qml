import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "."

Container {
    id: connectionButtonPanel
    property var connected: false
    property var running: false

    property color tint: null
    property bool hovering: false
    property string tooltip: ""

    onConnectedChanged: updateState();
    onRunningChanged: updateState();
    onHoveringChanged: updateState();

    Layout.preferredHeight: 199
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        id: backgroundRect
        color: Style.panelBackgroundColor
    }

    // static outline
    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/res/images/connection_button_outline.png"
    }

    // white connection image; we will colorize with LevelAdjust to indicate
    // status
    Image {
        id: connectionButtonImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        smooth: true
        source: "qrc:/res/images/connection_button.png"
    }

    // colorize connectionButtonImage
    LevelAdjust {
        id: level
        anchors.fill: connectionButtonImage
        source: connectionButtonImage
        maximumOutput: tint
    }

    // handle user interaction
    MouseArea {
        id: panelMa
        anchors.fill: connectionButtonImage
        hoverEnabled: true
        onEntered: {
            hovering = true;
        }
        onExited: {
            hovering = false;
        }
        onPressed: {
            if (! connected) {
                platformDetails.startLokinetProcess();
            } else {
                if (! running) {
                    apiClient.llarpAdminWakeup(function(response, err) {
                        if (err) {
                            console.log("Received error when trying to wakeup lokinet daemon: ", err);
                        }
                    });
                } else {
                    // TODO: tell daemon to pause
                }
            }
        }
    }
    ToolTip {
        id: statusTooltip
        parent: connectionButtonImage
        text: tooltip
        delay: 250
        visible: panelMa.containsMouse
    }

    function updateState() {
        console.log("updateState(), connected: "+ connected);

        // highlight on hover
        const lighter = (hovering ? 1.25 : 1.0);

        var base = "";
        if (connected && running) {
            base = Style.highlightAffirmative;
            tooltip = "Currently connected.\nPress to pause connection to the Loki Network.\n(not yet implemented)"
        } else if (connected) {
            // connected, but not running
            base = Style.highlightNeutral;
            tooltip = "Currently paused.\nPress to resume connection to the Loki Network."
        } else {
            // not connected
            base = Style.highlightNegative
            tooltip = "Unable to connect to the Loki\nNetwork daemon, is it running?";
        }
        tint = Qt.lighter(base, lighter);
    }
}
