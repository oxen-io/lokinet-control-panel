import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    property var down: 0
    property var up: 0

    Layout.preferredHeight: 79
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    // "Usage" label
    Text {
        x: 20
        y: 10
        text: "Usage"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }

    // Download
    Text {
        x: 20
        y: 32
        text: "Download"
        font.family: Style.strongTextFont
        color: Style.strongTextColor
        font.pointSize: Style.strongTextSize
    }
    Text {
        x: 20
        y: 54
        text: ""+ (down / (2^20)) +" MB" // TODO: pick appropriate scale
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

    // Upload
    Text {
        x: 150
        y: 32
        text: "Upload"
        font.family: Style.strongTextFont
        color: Style.strongTextColor
        font.pointSize: Style.strongTextSize
    }
    Text {
        x: 150
        y: 54
        text: ""+ (up / (2^20)) +" MB" // TODO: pick appropriate scale
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

}

