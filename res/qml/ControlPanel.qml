import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

ColumnLayout {
    anchors.fill: parent
    spacing: 1

    property var isConnected: true

    // other colors
    // strong text: #FAF9FA
    // weak text: #7D8585

    ConnectionHeaderPanel {
        connected: isConnected
    }

    // placeholder for main on/off button
    ConnectionButtonPanel {
        connected: isConnected
    }

    // placeholder for address text panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 79
        Layout.preferredWidth: 300
    }

    // placeholder for paths / routers text panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 69
        Layout.preferredWidth: 300
    }

    // placeholder for performance graph panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 159
        Layout.preferredWidth: 300
    }

    // placeholder for usage panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 79
        Layout.preferredWidth: 300
    }

    // placeholder / empty space
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 79
        Layout.preferredWidth: 300
    }

    // placeholder for dismiss-button panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 39
        Layout.preferredWidth: 300
    }

}

