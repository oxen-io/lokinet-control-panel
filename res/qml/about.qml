import QtQuick 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.2

ApplicationWindow {
    id: about
    title: qsTr("About Lokinet Control Panel")
    visible: false
    color: Style.panelSeparatorColor
    width: 320
    height: 240

    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.centerIn: parent
        spacing: 4
        anchors.margins: 16

        Image {
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            Layout.alignment: Qt.AlignCenter
            source: "qrc:/res/images/lokinet-brand_icon-only.png"
        }

        Text {
            Layout.alignment: Qt.AlignCenter
            color: Style.weakTextColor
            text: qsTr("Lokinet Control Panel")
        }
        Text {
            Layout.alignment: Qt.AlignCenter
            color: Style.weakTextColor
            text: qsTr("Version 0.6.2")
        }
        Text {
            Layout.alignment: Qt.AlignCenter
            color: Style.weakTextColor
            linkColor: Style.linkColor
            text: '<html>Website: <a href="https://lokinet.org">lokinet.org</a></html>'
            onLinkActivated: Qt.openUrlExternally(link)
        }
        Text {
            Layout.alignment: Qt.AlignCenter
            color: Style.weakTextColor
            linkColor: Style.linkColor
            text: '<html>Copyright 2020 <a href="https://loki.network">Rangeproof LTD</a><html>'
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
