import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "."

Container {
    id: versionPanel
    property var version: ""

    Layout.preferredHeight: 69
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        id: backgroundRect
        color: Style.panelBackgroundColor
    }

    ColumnLayout {
        id: versionTextGroup
        anchors.left: parent.left
        anchors.top: parent.top
        spacing: 4
        anchors.margins: 16


        // "version"
        Text {
            id: versionLabelText

            text: "Version"
            font.family: Style.weakTextFont
            color: Style.weakTextColor
            font.pointSize: Style.weakTextSize
            font.capitalization: Font.AllUppercase
        }
        // version text
        Text {
            id: versionText

            text: version
            font.family: Style.strongTextFont
            color: Style.strongTextColor
            font.pointSize: Style.strongTextSize
        }
    }
}

