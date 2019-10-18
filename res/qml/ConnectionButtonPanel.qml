import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
import QtQuick.Layouts 1.11

import "."

Container {
    id: connectionButtonPanel
    property var connected: false
    property var running: false

    property color tint: null
    property bool hovering: false
    property url iconUrl: null
    property string status: "Unknown"
    property string tooltip: ""

    onConnectedChanged: updateState();
    onRunningChanged: updateState();
    onHoveringChanged: updateState();

    Layout.preferredHeight: 59
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        id: backgroundRect
        color: Style.panelBackgroundColor
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        spacing: 4
        anchors.margins: 16


        // "Status"
        Text {
            id: statusLabelText

            text: "Status"
            font.family: Style.weakTextFont
            color: Style.weakTextColor
            font.pointSize: Style.weakTextSize
            font.capitalization: Font.AllUppercase
        }
        // status text
        Text {
            id: statusText

            text: status
            font.family: Style.weakTextFont
            color: tint
            font.pointSize: Style.weakTextSize
        }
    }

    // tooltip
    ToolTip {
        id: statusTooltip
        text: tooltip
        delay: 250
        visible: panelMa.containsMouse
    }

    // white connection image; we will colorize with LevelAdjust to indicate
    // status
    Image {
        id: connectionIndicator
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16
        width: 32
        height: 32
        smooth: true
        source: iconUrl
    }

    // colorize connectionIndicator
    LevelAdjust {
        id: level
        anchors.fill: connectionIndicator
        source: connectionIndicator
        maximumOutput: tint
    }

    // handle user interaction
    MouseArea {
        id: panelMa
        anchors.fill: parent
        cursorShape: (connected ? Qt.PointingHandCursor : Qt.ArrowCursor)
        enabled: true
        hoverEnabled: true
        onEntered: {
            hovering = true;
        }
        onExited: {
            hovering = false;
        }
        onPressed: {
            if (connected) {
                if (running) {
                    console.log("TODO: implement suspending lokinet");
                } else {
                    apiClient.llarpAdminWakeup(function(response, err) {
                        if (err) {
                            console.log("Received error when trying to wakeup lokinet daemon: ", err);
                        }
                    });
                }
            }
        }
    }

    function updateState() {
        console.log("updateState(), connected: "+ connected);

        // highlight on hover
        const lighter = (hovering ? 1.25 : 1.0);

        let base = "";
        if (connected && running) {
            base = Style.highlightAffirmative
            iconUrl = "qrc:/res/images/pause-circle.svg"
            status = "Connected"
            tooltip = "Currently connected.\nPress to pause connection to the Loki Network.\n(not yet implemented)"
        } else if (connected) {
            // connected, but not running
            base = Style.highlightNeutral
            iconUrl = "qrc:/res/images/play-circle.svg"
            status = "Paused"
            tooltip = "Currently paused.\nPress to resume connection to the Loki Network."
        } else {
            // not connected
            base = Style.highlightNegative
            iconUrl = "qrc:/res/images/warning.svg"
            status = "Disconnected"
            tooltip = "Unable to connect to the Loki\nNetwork daemon, is it running?"
        }
        tint = Qt.lighter(base, lighter);
    }
}

