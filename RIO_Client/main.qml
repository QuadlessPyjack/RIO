import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

import RIOClient.AppManager 1.0

import "Windows"
import "Components"

Window {
    visible: true
    width: 310
    height: 600
    title: qsTr("RIO Client")

    Component.onCompleted: {
        console.log("Window loaded!");
    }

    Connections {
        target: AppManager
        onConnectedToServer: {
            btnConnection.isServerConnected = true;
            btnConnection.text = "Disconnect";
            console.log("isServerConnected: " + btnConnection.isServerConnected);
        }

        onDisconnectedFromServer: {
            btnConnection.isServerConnected = false;
            btnConnection.text = "Connect";
            console.log("isServerConnected: " + btnConnection.isServerConnected);
        }
    }

    Button {
        id: btnConnection
        property bool isServerConnected: false
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10

        text: "Connect"
        onClicked: {
            //console.log("server connected: " + isServerConnected);
            if(isServerConnected) {
                text = "Connect";
                AppManager.disconnectFromServer();
            } else {
                text = "Disconnect";
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

            labelText: "Transmit Info Interval (s):"
            textInput: "5"
            buttonCaption: "Apply"
            onButtonClicked: {
                AppManager.transmitInterval = textInput;
                console.log("Setting timeout to: " + textInput);
            }
        }

        InputField {
            Layout.fillWidth: true
            Layout.preferredHeight: 15

            labelText: "Message:"
            placeholderText: "Write message to send here..."
            buttonCaption: "Send"
            onButtonClicked: {
                AppManager.inputMessage = textInput;
                AppManager.inputMessageChanged();
                console.log("Message input is: " + textInput);
            }
        }

        Button {
            id: btnDetailsWindow
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 10

            text: "Connection details..."
            onClicked: {
                windowConnectionDetails.show();
            }
        }
    }

    DetailsWindow {
        id: windowConnectionDetails
    }
}
