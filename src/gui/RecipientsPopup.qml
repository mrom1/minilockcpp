
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Popup {
    id: recipientsPopup
    width: parent.width*3/4
    anchors.centerIn: parent
    clip: true

    property var recipients: []

    ColumnLayout {
        width: parent.width
        Layout.margins: 20
        TextField {
            id: id1
            Layout.fillWidth: true
            text: QMinilock.id
            selectByMouse: true
        }
        TextField {
            id: id2
            Layout.fillWidth: true
            placeholderText: "Enter recipient ID ..."
            selectByMouse: true
        }
        TextField {
            id: id3
            Layout.fillWidth: true
            placeholderText: "Enter recipient ID ..."
            selectByMouse: true
        }
        TextField {
            id: id4
            Layout.fillWidth: true
            placeholderText: "Enter recipient ID ..."
            selectByMouse: true
        }
        TextField {
            id: id5
            Layout.fillWidth: true
            placeholderText: "Enter recipient ID ..."
            selectByMouse: true
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Button {
                text: "Ok"
                onClicked: {
                    recipients = []
                    recipients.push(id1.text)
                    recipients.push(id2.text)
                    recipients.push(id3.text)
                    recipients.push(id4.text)
                    recipients.push(id5.text)
                    QMinilock.recipients = recipients
                    recipientsPopup.close()
                }
            }
            Button {
                text: "Close"
                onClicked: recipientsPopup.close()
            }
        }
    }
}
