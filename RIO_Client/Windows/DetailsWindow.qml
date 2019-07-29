import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.3

import RIOClient.AppManager 1.0

Item {
    function show() {
        detailsWindow.show();
    }

    Component.onCompleted: {
        AppManager.updateLatencyPlotData(chartView.series(0));
        AppManager.setDisconnectSeriesPtr(chartView.series(1));
    }

    Connections {
        target: AppManager
        onLatencyChanged: {
            AppManager.clearLatencyPlotData(chartView.series(0));
            AppManager.updateLatencyPlotData(chartView.series(0));
        }
    }

    Window {
        id: detailsWindow
        width: 640
        height: 480
        title: qsTr("Connection details")

        Column {
            anchors.fill: parent
            Rectangle {
                color: "lightgray"
                border.color: "black"
                width: parent.width
                height: parent.height * 0.3

                Column {
                    spacing: 5
                        Text {
                            text: qsTr("Disconnects from user: ") + AppManager.userDisconnects
                        }
                        Text {
                            property int serverDisconnects: parseInt(AppManager.serverDisconnects) - parseInt(AppManager.userDisconnects)
                            text: qsTr("Disconnects from server: ") + serverDisconnects
                        }
                        Text {
                            text: qsTr("Disconnects total: ") + AppManager.serverDisconnects
                        }
                }
            }

            ChartView {
                id: chartView
                width: parent.width
                height: parent.height * 0.6
                theme: ChartView.ChartThemeBlueIcy
                antialiasing: true
                title: qsTr("Latency")

                ValueAxis {
                    id: vaX
                    min: 0
                    max: AppManager.latencyXMax
                }

                ValueAxis {
                    id: vaY
                    min: 0
                    max: AppManager.latencyYMax
                }

                LineSeries {
                    id: lineSeries
                    name: "μs"
                    axisX: vaX
                    axisY: vaY
                }

                LineSeries {
                    id: disconnectSeries
                    name: qsTr("disconnections")
                    axisX: vaX
                    axisY: vaY
                    color: "red"
                }
            }
        }
    }
}
