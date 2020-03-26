import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

import QClipboard 1.0
import "."

Container {
    Layout.preferredHeight: 59
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: "#101010"
    }
    Image {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        // coerces Qt not to interpolate like an idiot
        mipmap: true

        anchors.fill: parent

        fillMode: Image.PreserveAspectFit

        source: "qrc:/res/images/banner.png"
        smooth: true
    }
}

