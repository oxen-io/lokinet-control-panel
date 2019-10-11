import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

Container {
    property var connected: false
    property var running: false

    property color tint: "#000000"
    property var title: ""

    // bind properties that affect color to updateColor(), which will
    // update our tint property
    onConnectedChanged: updateState();
    onRunningChanged: updateState();

    Layout.preferredHeight: 59
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        id: connectedStatusRect
        color: tint
    }

    Text {
        id: connectedStatusText
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: title
        font.family: "Ubuntu"
        color: "#FAF9FA"
        font.pointSize: 15
    }

    function updateState() {

        let newColor = "#000000";
        let newTitle = "";

        if (connected && running) {
            newTitle = "CONNECTED";
            newColor = "#55CE53";
        } else if (connected) {
            // connected, but not running
            newTitle = "STOPPED";
            newColor = "#CECC53";
        } else {
            // not connected
            newTitle = "DISCONNECTED";
            newColor = "#CE5355";
        }

        if (newColor !== tint) tint = newColor;
        if (newTitle != title) title = newTitle;
    }
}

