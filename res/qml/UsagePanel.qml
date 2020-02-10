import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtCharts 2.0

import QClipboard 1.0
import BandwidthChartData 1.0
import "."

Container {
    property var down: 0
    property var up: 0

    BandwidthChartData {
        id: chartData
    }

    onUpChanged: function() {
        // console.log("new 'up' value: "+ up);
        chartData.addUploadSample(up);
        chartData.updateUploadSeries(txData.upperSeries);
    }

    onDownChanged: function() {
        // console.log("new 'down' value: "+ down);
        chartData.addDownloadSample(down);
        chartData.updateDownloadSeries(rxData.upperSeries);
    }

    Layout.preferredHeight: 249
    Layout.preferredWidth: Style.appWidth

    contentItem: Rectangle {
        color: Style.panelBackgroundColor
    }

    function makeRate(value)
    {
      var unit_idx = 0;
      var units = ["B", "KB", "MB"];
      while(value > 1024.0 && ( unit_idx + 1 ) < units.length)
      {
        value /= 1024.0;
        unit_idx += 1;
      }
      return "" + Math.round(value) + units[unit_idx] + "/s";
    }

    // "Usage" label
    Text {
        x: 20
        y: 10
        text: "Usage"
        font.family: Style.weakTextFont
        color: Style.weakTextColor
        font.pointSize: Style.weakTextSize
        font.capitalization: Font.AllUppercase
    }

    // Download
    Text {
        x: 20
        y: 32
        text: "Download"
        font.family: Style.strongTextFont
        color: Style.strongTextColor
        font.pointSize: Style.strongTextSize
    }
    Text {
        x: 20
        y: 54
        text: makeRate(down)
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

    // Upload
    Text {
        x: 150
        y: 32
        text: "Upload"
        font.family: Style.strongTextFont
        color: Style.strongTextColor
        font.pointSize: Style.strongTextSize
    }
    Text {
        x: 150
        y: 54
        text: makeRate(up)
        font.family: Style.weakTextFont
        color: Style.strongTextColor
        font.pointSize: Style.weakTextSize
    }

    ChartView {
        id: chart
        title: ""
        // anchors.fill:  parent
        // anchors.margins: 0
        antialiasing: true
        backgroundColor: Style.panelBackgroundColor
        theme: ChartView.ChartThemeDark

        legend.visible: false

        // these weird numbers come from an attempt to work around ChartView's
        // nasty permanent margins
        x: -10
        y: 70
        width: Style.appWidth + 20
        height: 189

        ValueAxis {
            id: xAxis
            labelFormat: "%d"
            min: 0
            max: 128
            labelsVisible: false
            gridVisible: false
            titleVisible: false
        }

        ValueAxis {
            id: yAxis
            min: 0
            max: 10000
            gridVisible: false
            labelFormat: "%d"
            titleVisible: false
        }

        AreaSeries {
            id: "txData"
            axisX: xAxis
            axisY: yAxis
            opacity: 0.8
            upperSeries: LineSeries {}

        }

        AreaSeries {
            id: "rxData"
            axisX: xAxis
            axisY: yAxis
            opacity: 0.8
            upperSeries: LineSeries {}
        }
    }
}

