import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

Container {
    property var connected: false

    Layout.preferredHeight: 199
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        id: backgroundRect
        color: "#323642"
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: (connected
            ? "qrc:/res/images/connected_indicator.png"
            : "qrc:/res/images/disconnected_indicator.png")
             
    }
}

