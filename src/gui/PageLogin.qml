/************************************************************************
 * This file is part of the minilockcpp distribution
 * (https://github.com/mrom1/minilockcpp).
 * Copyright (c) 2021 mrom1.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/
 


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
                if(QMinilock.passwordEntropy < 70)
                    strength = "Too Weak!"
                else if (QMinilock.passwordEntropy > 70 && QMinilock.passwordEntropy < 150)
                    strength = "Good enough!"
                else if (QMinilock.passwordEntropy > 150)
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
            inputMethodHints: Qt.ImhEmailCharactersOnly
            anchors.horizontalCenter: parent.horizontalCenter
            //Layout.fillWidth: true
            width: parent.width*2/3
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
                        return "icons/show_password_disabled.svg"
                    else
                        return "icons/show_password_enabled.svg"
                }
                Layout.maximumHeight: 30
                Layout.maximumWidth: 30

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
                if(input_email.text.length > 0 && input_password.text.length > 0 && QMinilock.passwordEntropy > 70)
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


