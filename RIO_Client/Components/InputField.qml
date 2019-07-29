import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: root
    property string labelText
    property string placeholderText
    property string buttonCaption
    property alias textInput: txtField.text
    property alias buttonEnabled: btnInputField.enabled
    signal buttonClicked()


    Rectangle {
        Column {
            anchors.fill: parent
            spacing: 5
            Text {
                id: msgField
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                text: labelText
                font.pointSize: 7
            }
            Row {
                spacing: 5
                TextField {
                    id: txtField
                    placeholderText: root.placeholderText
                }
                Button {
                    id: btnInputField
                    text: buttonCaption
                    onClicked: {
                        root.buttonClicked();
                    }
                }
            }
        }
    }
}
