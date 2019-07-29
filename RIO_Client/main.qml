import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

import RIOClient.AppManager 1.0

import "Windows"
import "Components"

Window {
    id: root
    visible: true
    width: 310
    height: 600
    title: qsTr("RIO Client")
    property bool isConnected: false

    Connections {
        target: AppManager
        onConnectedToServer: {
            root.isConnected = true;
            btnConnection.text = qsTr("Disconnect");
            console.log("isServerConnected: " + btnConnection.isServerConnected);
        }

        onDisconnectedFromServer: {
            root.isConnected = false;
            btnConnection.text = qsTr("Connect");
            console.log("isServerConnected: " + btnConnection.isServerConnected);
        }
    }

    Button {
        id: btnConnection
        property bool isServerConnected: root.isConnected
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10

        text: qsTr("Connect")
        onClicked: {
            if(isServerConnected) {
                text = qsTr("Connect");
                AppManager.disconnectFromServer();
            } else {
                text = qsTr("Disconnect");
                AppManager.connectToServer();
            }
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        width: parent.width * 0.7
        height: parent.height * 0.7
        spacing: 2

        InputField {
            Layout.fillWidth: true
            Layout.preferredHeight: 15

            labelText: qsTr("Transmit Info Interval (s):")
            textInput: "5"
            buttonCaption: qsTr("Apply")
            onButtonClicked: {
                AppManager.transmitInterval = textInput;
                console.log("Setting timeout to: " + textInput);
            }
        }

        InputField {
            Layout.fillWidth: true
            Layout.preferredHeight: 15
            buttonEnabled: root.isConnected
            labelText: qsTr("Message:")
            placeholderText: qsTr("Write message to send here...")
            buttonCaption: qsTr("Send")
            onButtonClicked: {
                AppManager.inputMessage = textInput;
                AppManager.inputMessageChanged();
            }
        }

        Button {
            id: btnDetailsWindow
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 10

            text: qsTr("Connection details...")
            onClicked: {
                windowConnectionDetails.show();
            }
        }
    }

    DetailsWindow {
        id: windowConnectionDetails
    }
}
