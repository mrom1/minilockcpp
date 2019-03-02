#pragma once

#include <QObject>
#include <QThread>

#include <minilock.h>

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(minilockcpp::minilock* session, QList<QString> fileNames, bool randomFilename,
           QString sourceDirectory, QString targetDirectory,
           std::vector<std::string> recipients) : session(session), fileNames(fileNames), randomFilename(randomFilename), sourceDirectory(sourceDirectory),
        targetDirectory(targetDirectory), recipients(recipients)
    {
    }

    Worker(minilockcpp::minilock* session, QList<QString> fileNames, QString sourceDirectory, QString targetDirectory)
        : session(session), fileNames(fileNames), sourceDirectory(sourceDirectory), targetDirectory(targetDirectory)
    {
    }

public slots:
    void encryptFiles()
    {
        for(auto file : fileNames) {
            if(QThread::currentThread()->isInterruptionRequested())
                return;
            QString status = "Encrypting " + file + " ... ";
            emit statusTextChanged(status);
            try {
                if(session->encrypt_file(file.toStdString(), randomFilename, sourceDirectory.toStdString(), targetDirectory.toStdString(), recipients))
                {
                    status = "OK!\n";
                    emit statusTextChanged(status);
                    emit progressedFileFinished();
                }
            }
            catch(std::exception& e) {
                status = "FAIL!\n[->] ";
                status += e.what();
                status += "\n";
                emit statusTextChanged(status);
            }
        }
    }

    void decryptFiles()
    {
        for(auto file : fileNames) {
            if(QThread::currentThread()->isInterruptionRequested())
                return;
            QString status = "Decrypting " + file + " ... ";
            emit statusTextChanged(status);
            try {
                if(session->decrypt_file(file.toStdString(), sourceDirectory.toStdString(), targetDirectory.toStdString()))
                {
                    status = "OK!\n";
                    emit statusTextChanged(status);
                    emit progressedFileFinished();
                }
            }
            catch(std::exception& e) {
                status = "FAIL!\n[->] ";
                status += e.what();
                status += "\n";
                emit statusTextChanged(status);
            }
        }
    }


signals:
    void progressedFileFinished();
    void statusTextChanged(QString);

private:
    minilockcpp::minilock* session;
    QList<QString> fileNames;
    bool randomFilename;
    QString sourceDirectory;
    QString targetDirectory;
    std::vector<std::string> recipients;
};
