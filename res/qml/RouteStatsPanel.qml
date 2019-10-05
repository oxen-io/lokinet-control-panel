import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0

Container {
    property var paths: 0
    property var routers: 0

    Layout.preferredHeight: 69
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        color: "#323642"
    }

    // "Paths built"
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 18
        text: "Paths built"
        font.family: "Ubuntu Condensed"
        color: "#7D8585"
        font.pointSize: 11
        font.capitalization: Font.AllUppercase
    }
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 40
        text: ""+ paths
        font.family: "Ubuntu"
        color: "#FAF9FA"
        font.pointSize: 11
    }

    // "Lokinet Routers"
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 170
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 18
        text: "Lokinet routers"
        font.family: "Ubuntu Condensed"
        color: "#7D8585"
        font.pointSize: 11
        font.capitalization: Font.AllUppercase
    }
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 170
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 40
        text: ""+ routers
        font.family: "Ubuntu"
        color: "#FAF9FA"
        font.pointSize: 11
    }
}

