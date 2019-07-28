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
            console.log("Updating latency points");
            AppManager.clearLatencyPlotData(chartView.series(0));
            AppManager.updateLatencyPlotData(chartView.series(0));
            //chartView.zoom(0.0);
            //chartView.update();
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
                color: "gray"
                border.color: "black"
                width: parent.width
                height: parent.height * 0.3

                Column {
                        Text {
                            text: "Disconnects from user: " + AppManager.userDisconnects
                        }
                        Text {
                            var serverDisconnects = parseInt(AppManager.serverDisconnects) - parseInt(AppManager.userDisconnects)
                            text: "Disconnects from server: " + serverDisconnects
                        }
                        Text {
                            text: "Disconnects total: " + AppManager.serverDisconnects
                        }
                }
            }

            ChartView {
                id: chartView
                width: parent.width
                height: parent.height * 0.6
                theme: ChartView.ChartThemeBlueIcy
                antialiasing: true
                title: "Latency"

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
                    name: "microseconds"
                    axisX: vaX
                    axisY: vaY
                }

                LineSeries {
                    id: disconnectSeries
                    name: "disconnections"
                    axisX: vaX
                    axisY: vaY
                    color: "red"
                }
            }
        }
    }
}
