import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

Container {
    property var connected: false

    Layout.preferredHeight: 59
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        id: connectedStatusRect
        color: (connected ? "#55CE53" : "red")
    }

    Text {
        id: connectedStatusText
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: (connected ? "CONNECTED" : "DISCONNECTED")
        font.family: "helvetica"
        color: "#FAF9FA"
        font.pointSize: 15
    }
}

