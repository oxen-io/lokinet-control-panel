import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import QClipboard 1.0
import "."

Container {
  property var address: ""
  property var authcode: ""
  property var status: ""
  property var busy: false
  property var hasExit: false
    Layout.preferredHeight: 200
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

    TextField {
        id: exitTextInput
        background: Rectangle{
          color: Style.textInputBackgroundColor
        }
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 40
        text: address
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

    Text {
        id: authLabelText

        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 80
        text: "Auth Code"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }

    TextField {
        id: authTextInput

        background: Rectangle{
          color: Style.textInputBackgroundColor
        }
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        y: 100
        text: authcode
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
        echoMode: TextInput.Password
    }

    Switch {
      id: exitButton
      palette.dark: Style.lokiDarkGreen;
      text: address.length > 0  ? "Exit On" : "Exit off"
      checkable: true
      checked: address.length > 0
      background: Rectangle{
        color: Style.panelBackgroundColor
        opacity: exitButton.checked ? 1 : 0.3
        border.color: Style.lokiDarkGreen
        border.width: exitButton.checked ? 2 : 0
        radius: 10
      }

      anchors.left: parent.left
      anchors.leftMargin: 20
      anchors.right: parent.right
      anchors.rightMargin: 20

      contentItem: Text{
        text: exitButton.text
        color: Style.lokiFontWhite
        font.family: Style.weakTextFont
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

      }

      y: 140
      onClicked: {
        if(busy)
        {
          return;
        }
        let exitAddr = exitTextInput.text;
        let exitAuth = authTextInput.text;
        if(hasExit || address.length > 0)
        {
          busy = true;
          apiClient.llarpDelExit(function(result, error) {
            status = "Exit Off";
            busy = false;
            hasExit = false;
            exitButton.text = "Enable Exit";
            stautsLabelText.color = Style.weakTextColor;
          });
          return;
        }
        statusLabelText.color = Style.weakTextColor;
        busy = true;
        apiClient.llarpAddExit(exitAddr, exitAuth, function(result, error) {
          busy = false;
          if(error)
          {
            status = "Error: " +error;
            statusLabelText.color = Style.errorRed;
            checked = false;
            return;
          }
          let j = JSON.parse(result);
          if(j.error)
          {
            status = "Error: " + j.error;
            checked = false;
            statusLabelText.color = Style.errorRed;
          }
          if(j.result)
          {
            stautsLabelText.color = Style.weakTextColor;
            status = "Exit Enabled";
            hasExit = true;
            exitButton.text = "Disable Exit";
            // apiClient.llarpSetConfig({"network": { "exit-node": exitAddr + (exitAuth.length > 0 ? ":" + exitAuth : "") }}, function(error, result) {} )
          }
        });
      }
    }

    Text {
      id: statusLabelText
      anchors.left: parent.left
      anchors.leftMargin: 20
      anchors.right: parent.right
      anchors.rightMargin: 20
      y: 180
      text: status
      font.family: Style.weakTextFont
      color: Style.weakTextColor
      font.pointSize: Style.weakTextSize
      font.capitalization: Font.AllUppercase
    }

}
