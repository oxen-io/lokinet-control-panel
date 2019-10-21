import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import "."

Container {
    id: uptimePanel
    property int uptime: 0

    Layout.preferredHeight: 69
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        id: backgroundRect
        color: Style.panelBackgroundColor
    }

    function convertUptimeToHumanReadable(uptime) {
        console.log("uptime: "+ uptime);
        const dayMillis = (24 * 3600 * 1000);

        const days = Math.floor(uptime / dayMillis);
        const hours = Math.floor(uptime / (3600 * 1000)) % 24;
        const minutes = Math.floor(uptime / (60 * 1000)) % 60;

        var uptimeStr = ""+ days + "d "
                          + hours + "h "
                          + minutes + "m";
        return uptimeStr;
    }

    ColumnLayout {
        id: uptimeTextGroup
        anchors.left: parent.left
        anchors.top: parent.top
        spacing: 4
        anchors.margins: 16


        // "uptime"
        Text {
            id: uptimeLabelText

            text: "Uptime"
            font.family: Style.weakTextFont
            color: Style.weakTextColor
            font.pointSize: Style.weakTextSize
            font.capitalization: Font.AllUppercase
        }
        // uptime text
        Text {
            id: uptimeText

            text: (uptime > 0 ? convertUptimeToHumanReadable(uptime) : "")
            font.family: Style.strongTextFont
            color: Style.strongTextColor
            font.pointSize: Style.strongTextSize
        }
    }
}

