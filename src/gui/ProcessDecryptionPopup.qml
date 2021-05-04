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

Popup {
    id: processDecryptionPopup
    width: parent.width*3/4
    anchors.centerIn: parent
    clip: true
    property bool processingActive: false

    FolderDialog {
        id: targetFolderDialog
        folder: folderDialog.folder
    }

    ColumnLayout {
        width: parent.width

        RowLayout {
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            TextField {
                id: targetPath
                enabled: false
                text: targetFolderDialog.folder
                Layout.fillWidth: true
            }
            Button {
                text: "..."
                Layout.maximumHeight: 35
                Layout.maximumWidth: 35
                onClicked: targetFolderDialog.open()
            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0
            to: fileModel.selectedFiles.length
            value: QMinilock.progressedFiles

            Connections {
                target: QMinilock
                onProgressedFilesChanged: {
                    progressBar.value = QMinilock.progressedFiles
                    if(QMinilock.progressedFiles === fileModel.selectedFiles.length) {
                        processDecryption.processingActive = false
                    }
                }
            }
        }

        Label {
            text: {
                var arrow = logView.visible? "\u25BC" : "\u25B6"
                if(fileModel.selectedFiles.length === 0) { return arrow + " No files selected!" }
                else { return arrow + " Decrypting file " + QMinilock.progressedFiles + " of " + fileModel.selectedFiles.length + " ..." }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: logView.visible = !logView.visible
            }
        }

        ScrollView {
            id: logView
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            visible: false

            TextArea {
                readOnly: true
                selectByMouse: true
                selectByKeyboard: true
                text: QMinilock.statusText
                color: "white"
                font.family: "Monospace"
                font.pixelSize: 11
                background: Rectangle {
                    color: "#5c5c5c"
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Button {
                text: "Start"
                onClicked: {
                    if(!processDecryption.processingActive) {
                        QMinilock.decryptFiles(fileModel.selectedFiles,
                                               folderDialog.urlToPath(folderModel.folder.toString()),
                                               folderDialog.urlToPath(targetFolderDialog.folder.toString()))
                        processDecryption.processingActive = true
                    }
                }
                enabled: !processDecryption.processingActive
            }

            Button {
                id: closeButton
                text: processDecryption.processingActive ? "Cancle" : "Close"
                onClicked: {
                    if(processDecryption.processingActive) {
                        QMinilock.cancleAll()
                        processDecryption.processingActive = false
                    }
                    else {
                        QMinilock.clearStatusText()
                        processDecryptionPopup.close()
                    }
                }
            }
        }
    }
}
