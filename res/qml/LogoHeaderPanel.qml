import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    Layout.preferredHeight: 59
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: "#101010"
    }

    RowLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 4
        Layout.fillWidth: false

        Image {
            Layout.minimumWidth: 36
            Layout.preferredWidth: 36
            Layout.minimumHeight: 36
            Layout.preferredHeight: 36

            source: "qrc:/res/images/icon.png"
        }

        // "LOKI"
        Text {
            text: "LOKI"
            font.family: Style.weakTextFont
            color: Style.logoTextcolor
            font.pointSize: Style.logoTextSize
            font.capitalization: Font.AllUppercase
        }
    }

}

