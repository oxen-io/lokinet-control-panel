import QtQuick 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0

import PlatformDetails 1.0
import "."

Dialog {
    id: dialog
    title: "Download bootstrap file"
    standardButtons: Dialog.Ok | Dialog.Cancel
    width: 400

    readonly property string defaultURL: "https://seed.lokinet.org/lokinet.signed"

    function isURLValid() {
        return urlInput.acceptableInput;
    }
    function getURL() {
        return urlInput.text;
    }
    function resetURL() {
        urlInput.text = defaultURL;
    }

    GridLayout {
        width: parent.width
        id: grid
        columns: 2

        Text {
            text: "URL:"
            font.bold: true;
        }
        TextField {
            id: urlInput
            Layout.fillWidth: true
            validator: RegExpValidator {
                // TODO: review this regex (now we have two problems...?)
                regExp: /https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*)/
            }
            focus: true

            text: defaultURL
        }
    }

    MessageDialog {
        id: alert
        title: ""
        text: ""
        visible: false
    }

    onAccepted: {
        if (! bootstrapDialog.isURLValid()) {
            alert.title = "Invalid input";
            alert.text = "Input must be a valid URL";
            alert.visible = true;

            bootstrapDialog.resetURL();
        } else {

            // TODO: a progress indicator of some sort

            var url = bootstrapDialog.getURL();
            console.log("Downloading bootstrap, URL: "+ url);
            platformDetails.downloadBootstrapFile(url, function(err, msg) {
                if (err) {
                    alert.title = "Bootstrap error "+ err;
                } else {
                    alert.title = "Bootstrap results";
                }
                alert.text = msg;
                alert.visible = true;
            });
        }
    }
}

