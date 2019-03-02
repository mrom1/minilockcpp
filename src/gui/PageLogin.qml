import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Page {
    width: parent.width
    height: parent.height

    footer: Item {
        id: footer
        height: 40
        Label {
            text: {
                var strength;
                if(QMinilock.passwordEntropy < 100)
                    strength = "Too Weak!"
                else if (QMinilock.passwordEntropy > 100 && QMinilock.passwordEntropy < 200)
                    strength = "Good enough!"
                else if (QMinilock.passwordEntropy > 200)
                    strength = "Very Strong!"

                qsTr("Password Strength: " + strength)
            }
            font.pixelSize: Qt.application.font.pixelSize
            padding: 10
            anchors.left: parent.left
        }
    }

    Column {
        spacing: 12
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id: minilock_image
            visible: true
            width: 128
            height: 128
            source: "icons/minilock.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }


        TextField {
            id: input_email
            placeholderText: "Enter your E-Mail address..."
            selectByMouse: true
            width: parent.width*2/3
            inputMethodHints: Qt.ImhEmailCharactersOnly
            anchors.horizontalCenter: parent.horizontalCenter
        }

        RowLayout {
            width: parent.width*2/3
            anchors.horizontalCenter: parent.horizontalCenter

            TextField {
                id: input_password
                placeholderText: "Enter your password..."
                echoMode: TextInput.Password
                selectByMouse: true
                Layout.fillWidth: true
                onTextChanged: QMinilock.setPasswordEntropy(input_password.text)
            }

            Image {
                id: icon_password
                property bool toggle: true
                source: {
                    if(toggle)
                        return "icons/show_hide_password.png"
                    else
                        return "icons/show_password.png"
                }
                Layout.maximumHeight: 25
                Layout.maximumWidth: 25

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(icon_password.toggle) {
                            input_password.echoMode = TextInput.Normal
                            icon_password.toggle = !icon_password.toggle
                        }
                        else {
                            input_password.echoMode = TextInput.Password
                            icon_password.toggle = !icon_password.toggle
                        }
                    }
                }
            }
        }

        Button {
            text: "login"
            enabled: {
                if(input_email.text.length > 0 && input_password.text.length > 0 && QMinilock.passwordEntropy > 100)
                    return true;
                else return false;
            }
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                if(QMinilock.initialize(input_email.text, input_password.text))
                {
                    input_password.text = ""
                    fsm.state = "PageMinilock"
                }
            }
        }
    }
}


