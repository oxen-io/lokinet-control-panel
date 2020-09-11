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
    Layout.preferredHeight: 162
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

        y: 62
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

        y: 84
        text: authcode
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
        echoMode: TextInput.Password
    }

    Text {
      id: statusLabelText
      anchors.left: parent.left
      anchors.leftMargin: 20
      anchors.right: parent.right
      anchors.rightMargin: 20
      y: 106
      text: status
      font.family: Style.weakTextFont
      color: Style.weakTextColor
      font.pointSize: Style.weakTextSize
      font.capitalization: Font.AllUppercase
    }

    Button {
      id: exitButton 
      text: "Enable Exit"
      checkable: true
      background: Rectangle{
        color: Style.highlightAffirmative
      }

      anchors.left: parent.left
      anchors.leftMargin: 20
      anchors.right: parent.right
      anchors.rightMargin: 20

      font.family: Style.weakTextFont
      font.pointSize: Style.weakTextSize
      font.capitalization: Font.AllUppercase
      y: 128
      onClicked: {
        if(busy)
        {
          return;
        }
        let exitAddr = exitTextInput.text;
        let exitAuth = authTextInput.text;
        console.log(exitAddr, exitAuth);
        if(hasExit)
        {
          busy = true;
          apiClient.llarpDelExit(function(result, error) {
            status = "Exit Off";
            busy = false;
            hasExit = false;
            exitButton.text = "Enable Exit";
          });
          return;
        }
        status = "Connecting...";
        busy = true;
        apiClient.llarpAddExit(exitAddr, exitAuth, function(result, error) {
          busy = false;
          if(error)
          {
            status = "Error: " +error;
            checked = false;
            return;
          }
          let j = JSON.parse(result);
          if(j.error)
          {
            status = "Error: " + j.error;
            checked = false;
          }
          if(j.result)
          {
            status = "Exit Enabled";
            hasExit = true;
            exitButton.text = "Disable Exit";
          }
        });
      }
    }    
}

