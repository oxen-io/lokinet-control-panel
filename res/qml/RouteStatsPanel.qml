import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    property var paths: 0
    property var routers: 0

    Layout.preferredHeight: 69
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    // "Paths built"
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 18
        text: "Paths built"
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

        y: 40
        text: ""+ paths
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

        y: 18
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

        y: 40
        text: ""+ routers
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }
}

