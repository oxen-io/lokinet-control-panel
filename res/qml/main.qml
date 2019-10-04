import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.1

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    TestPanel {
        
    }

    SystemTrayIcon {
        visible: true
        icon.source: "qrc:/res/images/icon.png"
    }

}
