import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import QClipboard 1.0
import "."

Container {
    property var paths: 0
    property var routers: 0
    property var buildRatio: ""

    Layout.preferredHeight: 45
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    // "Active Paths"
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 3
        text: "Active Paths"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 22
        text: ""+ paths + " (" + buildRatio + " success)"
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

    // "Lokinet Routers"
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 170
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 3
        text: "Lokinet routers"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 170
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 22
        text: ""+ routers
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }
}

