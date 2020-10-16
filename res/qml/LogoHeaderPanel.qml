import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import QClipboard 1.0
import "."

Container {
    property string version: ""

    Layout.preferredHeight: 79
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: "#101010"
    }

    ToolTip {
        text: version
        delay: 250
        visible: verMa.containsMouse
    }
    MouseArea {
        id: verMa
        anchors.fill: parent
        hoverEnabled: true
    }

    Image {
        id: logoImage
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        // coerces Qt not to interpolate like an idiot
        mipmap: true

        anchors.topMargin: 10
        anchors.bottomMargin: 10

        anchors.fill: parent

        fillMode: Image.PreserveAspectFit

        source: "qrc:/res/images/banner.png"
        smooth: true
    }
}

