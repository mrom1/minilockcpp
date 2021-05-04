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
