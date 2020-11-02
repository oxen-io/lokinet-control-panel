import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "."

Container {
    id: connectionButtonPanel
    property var running: false

    property color tint: null
    property bool hovering: false
    property string tooltip: ""

    onRunningChanged: updateState();
    onHoveringChanged: updateState();

    Layout.preferredHeight: 64
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
            if (! running) {
              running = platformDetails.startLokinetProcess();
            } else {
              running = !platformDetails.managedStopLokinetProcess();
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

        // highlight on hover
        const lighter = (hovering ? 1.25 : 1.0);

        var base = "";
        if (running) {
            base = Style.highlightAffirmative;
            tooltip = "Currently connected to the Loki Network."
        } else {
            // not connected
            base = Style.highlightNegative
            tooltip = "Unable to connect to the Loki\nNetwork daemon, is it running?";
        }
        tint = Qt.lighter(base, lighter);
    }
}
