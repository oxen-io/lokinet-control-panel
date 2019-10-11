import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    property var down: 0
    property var up: 0

    Layout.preferredHeight: 79
    Layout.preferredWidth: 300

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    // "Usage" label
    Text {
        x: 20
        y: 10
        text: "Usage"
        font.family: "Ubuntu"
        color: Style.weakTextColor
        font.pointSize: 11
        font.capitalization: Font.AllUppercase
    }

    // Download
    Text {
        x: 20
        y: 32
        text: "Download"
        font.family: "Ubuntu Condensed"
        color: Style.strongTextColor
        font.pointSize: 11
    }
    Text {
        x: 20
        y: 54
        text: ""+ (down / (2^20)) +" MB" // TODO: pick appropriate scale
        font.family: "Ubuntu"
        color: Style.strongTextColor
        font.pointSize: 11
    }

    // Upload
    Text {
        x: 150
        y: 32
        text: "Upload"
        font.family: "Ubuntu Condensed"
        color: Style.strongTextColor
        font.pointSize: 11
    }
    Text {
        x: 150
        y: 54
        text: ""+ (up / (2^20)) +" MB" // TODO: pick appropriate scale
        font.family: "Ubuntu"
        color: Style.strongTextColor
        font.pointSize: 11
    }

}

