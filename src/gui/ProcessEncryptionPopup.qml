import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.labs.folderlistmodel 2.12
import Qt.labs.platform 1.0

Popup {
    id: processEncryptionPopup
    width: parent.width*3/4
    anchors.centerIn: parent
    clip: true
    property bool processingActive: false

    FileDialog {
        id: targetFileDialog
        folder: folderDialog.folder
    }

    FolderDialog {
        id: targetFolderDialog
        folder: folderDialog.folder
    }

    ColumnLayout {
        width: parent.width
        RowLayout {
            CheckBox {
                id: checkBoxRandomFilenames
                text: "Use random filenames"
            }
            //CheckBox {
            //    id: checkBoxSelectedZIP
            //    text: "ZIP to one file"
            //}
        }

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
                onClicked: {
                    targetFolderDialog.open()
                }
            }
        }

        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            from: 0
            to: fileModel.selectedFiles.length

            Connections {
                target: QMinilock
                onProgressedFilesChanged: {
                    progressBar.value = QMinilock.progressedFiles
                    if(QMinilock.progressedFiles === fileModel.selectedFiles.length) {
                        processEncryption.processingActive = false
                    }
                }
            }
        }

        Label {
            text: {
                var arrow = logView.visible? "\u25BC" : "\u25B6"
                if(fileModel.selectedFiles.length === 0) { return arrow + " No files selected!" }
                else { return arrow + " Encrypting file " + QMinilock.progressedFiles + " of " + fileModel.selectedFiles.length + " ..." }
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
            clip: true

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
                    if(!processEncryption.processingActive) {
                        QMinilock.encryptFiles(fileModel.selectedFiles,
                                               checkBoxRandomFilenames.checked,
                                               folderDialog.urlToPath(folderModel.folder.toString()),
                                               folderDialog.urlToPath(targetFolderDialog.folder.toString()),
                                               recipientsPopup.recipients)
                        processEncryption.processingActive = true
                    }
                }
                enabled: !processEncryption.processingActive
            }

            Button {
                id: closeButton
                text: processEncryption.processingActive ? "Cancle" : "Close"
                onClicked: {
                    if(processEncryption.processingActive) {
                        QMinilock.cancleAll()
                        processEncryption.processingActive = false
                    }
                    else {
                        QMinilock.clearStatusText()
                        processEncryptionPopup.close()
                    }
                }
            }
        }
    }
}
