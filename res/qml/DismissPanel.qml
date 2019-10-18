import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

import QClipboard 1.0
import "."

Container {
    Layout.preferredHeight: 39
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/res/images/up_chevron.png"
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        onClicked: {
            window.hide();
        }
    }

}

