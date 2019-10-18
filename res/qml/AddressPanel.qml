import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    property var address: ""

    Layout.preferredHeight: 79
    Layout.preferredWidth: Style.appWidth

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
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
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
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
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
        hoverEnabled: (address && address != "")
        cursorShape: ((address && address != "")
                        ? Qt.PointingHandCursor
                        : Qt.ArrowCursor)
        onClicked: {
            clipboard.setText(address);
        }
    }
}

