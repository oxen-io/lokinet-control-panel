import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    property var address: "unknown"

    Layout.preferredHeight: 79
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        id: connectedStatusRect
        color: Style.panelBackgroundColor
    }

    Text {
        id: addressLabelText

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 18
        text: "Lokinet Address"
        font.family: "Ubuntu"
        color: Style.weakTextColor
        font.pointSize: 11
        font.capitalization: Font.AllUppercase
    }

    Text {
        id: addressText

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 40
        text: address
        font.family: "Ubuntu"
        color: "#FAF9FA"
        font.pointSize: 11
        elide: Text.ElideMiddle
    }

    ToolTip {
        text: "Click to copy Loki address"
        delay: 250
        visible: ma.containsMouse
    }

    QClipboard {
        id: clipboard
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            clipboard.setText(address);
        }
    }
}

