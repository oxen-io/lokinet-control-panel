import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

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
        id: exitLabelText

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 18
        text: "Exit Node"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }

    TextInput {
        id: exitTextInput

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 40
        text: address
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
        onAccepted: {
          let lokiExit = text;
          apiClient.llarpExit(lokiExit, function(result, error) {
              if(error)
                 console.log(error);
               else
                 console.log(JSON.stringify(result));
          });
          
        }
    }
}

