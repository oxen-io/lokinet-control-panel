import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

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

    function makeRate(value)
    {
      var unit_idx = 0;
      var units = ["B", "KB", "MB"];
      while(value > 1024.0 && ( unit_idx + 1 ) < units.length)
      {
        value /= 1024.0;
        unit_idx += 1;
      }
      return "" + Math.round(value) + units[unit_idx] + "/s";
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
        text: makeRate(down)
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
        text: makeRate(up)
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

}

