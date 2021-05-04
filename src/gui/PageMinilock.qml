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
import Qt.labs.folderlistmodel 2.12
import Qt.labs.platform 1.0
import QtQml.Models 2.12

Page {
    width: parent.width
    height: parent.height
    visible: true

    ProcessEncryptionPopup {
        id: processEncryption
    }

    ProcessDecryptionPopup {
        id: processDecryption
    }

    RecipientsPopup {
        id: recipientsPopup
    }

    header: RowLayout {
        id: header
        ColumnLayout {
            Label {
                id: headerEmail
                text: QMinilock.email
                font.pixelSize: Qt.application.font.pixelSize * 1.25
                padding: 10
            }

            TextField {
                id: labelHeader
                readOnly: true
                Layout.preferredWidth: contentWidth
                Layout.fillWidth: true
                Layout.leftMargin: 10
                text: { "ID: " + QMinilock.id }
                selectByMouse: true
            }
        }

        Image {
            id: logoutImage
            Layout.maximumHeight: 40
            Layout.maximumWidth: 40
            Layout.rightMargin: 10
            Layout.topMargin: 35
            Layout.alignment: Qt.AlignRight
            source: "icons/logout.svg"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    QMinilock.logout()
                    fileModel.selectedFiles = []
                    fsm.state = "PageLogin"
                }
            }
        }
    }

    footer: Item {
        id: footer
        height: 45

        Label {
            text: { qsTr("Files selected: " + fileModel.selectedFiles.length) }
            font.pixelSize: Qt.application.font.pixelSize
            padding: 10
            anchors.left: parent.left
        }

        ComboBox {
            id: selectMode
            anchors.right: buttonRecipients.left
            anchors.rightMargin: 10
            model: ["encrypt", "decrypt"]
        }

        Button {
            id: buttonRecipients
            anchors.right: buttonStart.left
            anchors.rightMargin: 10
            text: qsTr("Recipients")
            onClicked: {
                recipientsPopup.open()
            }
        }

        Button {
            id: buttonStart
            anchors.right: parent.right
            anchors.rightMargin: 20
            text: qsTr("Start")
            onClicked: {
                if(selectMode.currentText == "encrypt") { QMinilock.progressedFiles = 0; processEncryption.open() }
                else if(selectMode.currentText == "decrypt") { QMinilock.progressedFiles = 0; processDecryption.open() }
            }
        }
    }

    ColumnLayout {
        id: columnFileSelection
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        RowLayout {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            TextField {
                id: path
                placeholderText: folderModel.folder
                Layout.fillWidth: true
                onTextChanged: {
                    var filter = "*"
                    for(var i = 0; i< text.length; i++)
                        filter+= text[i]
                    filter+="*"
                    folderModel.nameFilters = [filter]
                }
            }
            Button {
                text: "..."
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                onClicked: folderDialog.open();
            }
        }
    }

    FolderListModel {
        id: folderModel
        folder: folderDialog.folder
        showDirs: false
        caseSensitive: false
        sortField: "Name"
        onCountChanged: fileModel.updateFiles()
    }

    FolderDialog {
        id: folderDialog
        currentFolder: ""
        folder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]

        function urlToPath(urlString) {
            var s
            if (urlString.startsWith("file:///")) {
                var k = urlString.charAt(9) === ':' ? 8 : 7
                s = urlString.substring(k)
            } else {
                s = urlString
            }
            return decodeURIComponent(s);
        }
    }

    Frame {
        id: frameFiles
        anchors.top: columnFileSelection.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 20
        padding: 5

        DelegateModel {
            id: fileModel
            model: folderModel

            property var selectedFiles: []

            groups: [
                DelegateModelGroup { name: "selected" }
            ]

            function updateFiles() {
                for(var i = 0; i < folderModel.count; i++) {
                    var item = items.get(i)
                    var fileName = folderModel.get(i, "fileName")
                    if(selectedFiles.includes(fileName))
                    {
                        item.inSelected = true
                    }
                }
            }

            function selectAllFiles() {
                for(var i = 0; i < items.count; i++) {
                    var item = items.get(i)
                    item.inSelected = true
                }
                var tmp = []
                for(i = 0; i < folderModel.count; i++)
                {
                    var fileName = folderModel.get(i, "fileName")
                    tmp.push(fileName)
                    selectedFiles = tmp
                }
            }

            function deselectAllFiles() {
                for(var i = 0; i < items.count; i++) {
                    var item = items.get(i)
                    item.inSelected = false
                }
                selectedFiles = []
            }

            delegate: RowLayout {
                id: item
                width: parent.width

                CheckBox {
                    checked: item.DelegateModel.inSelected
                    onClicked: {
                        var tmp;
                        if(!item.DelegateModel.inSelected)
                        {   // trick to notify the change for file counter
                            // select file
                            tmp = fileModel.selectedFiles
                            tmp.push(fileName)
                            fileModel.selectedFiles = tmp
                        }
                        else {
                            // trick to notify the change for file counter
                            // deselect file
                            tmp = fileModel.selectedFiles
                            tmp.splice(tmp.indexOf(fileName), 1)
                            fileModel.selectedFiles = tmp
                        }
                        item.DelegateModel.inSelected = !item.DelegateModel.inSelected
                    }
                }

                TextField {
                    readOnly: true
                    Layout.minimumWidth: 80
                    Layout.preferredWidth: contentWidth
                    text: {
                        if(fileSize==0) return '0 Bytes';
                        var k = 1000,
                        dm = 2,
                        sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
                        i = Math.floor(Math.log(fileSize) / Math.log(k));
                        return parseFloat((fileSize / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
                    }
                }
                TextField {
                    readOnly: true
                    selectByMouse: true
                    Layout.fillWidth: true
                    cursorPosition: 0
                    text: fileName
                }
            }
        }

        ListView {
            id: fileListView
            anchors.fill: parent
            model: fileModel
            clip: true

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: contextMenu.open()
                Menu {
                    id: contextMenu
                    MenuItem {
                        text: "Select all files"
                        onTriggered: fileModel.selectAllFiles()

                    }
                    MenuItem {
                        text: "Deselect all files"
                        onTriggered: fileModel.deselectAllFiles()
                    }
                }
            }
        }
    }
}
