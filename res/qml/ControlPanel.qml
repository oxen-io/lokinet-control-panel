import QtQuick 2.0
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

ColumnLayout {
    anchors.fill: parent
    spacing: 1

    property var isConnected: false
    property var lokiAddress: "7ai911bi38ni95u3kzgn9yfz1etos3uso7etrapb7josdjgq6cio.loki"
    property var numPathsBuilt: 10
    property var numRoutersKnown: 652
    property var downloadUsage: (7 * (2^20))
    property var uploadUsage: (1 * (2^20))

    // other colors
    // strong text: #FAF9FA
    // weak text: #7D8585

    ConnectionHeaderPanel {
        connected: isConnected
    }

    // placeholder for main on/off button
    ConnectionButtonPanel {
        connected: isConnected
    }

    // address panel
    AddressPanel {
        address: lokiAddress
    }

    // router stats
    RouteStatsPanel {
        paths: numPathsBuilt
        routers: numRoutersKnown
    }

    // placeholder for performance graph panel
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 159
        Layout.preferredWidth: 300
    }

    // usage
    UsagePanel {
        down: downloadUsage
        up: uploadUsage
    }

    // placeholder / empty space
    Rectangle {
        color: "#323642"
        Layout.preferredHeight: 79
        Layout.preferredWidth: 300
    }

    // dismiss panel
    DismissPanel { }


    Component.onCompleted: {
        statFetcher.statusAvailable.connect(handleStats);
    }

    function handleStats(payload) {
        console.log("Updated, payload: "+ payload);
        isConnected = true;
    }
}

