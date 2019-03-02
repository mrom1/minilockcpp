#pragma once

#include <QObject>
#include <QThread>

#include <minilock.h>

class QMinilock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString statusText READ getStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(float passwordEntropy READ getPasswordEntropy NOTIFY passwordEntropyChanged)
    Q_PROPERTY(float progressedFiles READ getProgressedFiles WRITE setProgressedFiles NOTIFY progressedFilesChanged)
    Q_PROPERTY(bool processingFinished READ processingFinished NOTIFY processingFinishedChanged)
    Q_PROPERTY(std::vector<QString> recipients WRITE setRecipients NOTIFY recipientsChanged)

public:
    explicit QMinilock (QObject* parent = nullptr) : QObject(parent) { }

    Q_INVOKABLE bool initialize(QString email, QString password);
    Q_INVOKABLE void encryptFiles(QList<QString> fileNames, bool random_filename, QString sourceDirectory, QString targetDirectory);
    Q_INVOKABLE void decryptFiles(QList<QString> fileNames, QString sourceDirectory, QString targetDirectory);
    Q_INVOKABLE void setPasswordEntropy(QString password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void cancleAll();
    Q_INVOKABLE void clearStatusText() { status_text = ""; emit statusTextChanged(); }

    QString getEmail() const  { return QString::fromStdString(session.get_email()); }
    QString getId() const { return QString::fromStdString(session.get_id()); }
    QString getStatusText() const { return status_text; }
    float getPasswordEntropy() const { return password_entropy; }
    float getProgressedFiles() const { return progressed_files; }
    bool processingFinished() const { return processing_finished; }
    void setProgressedFiles(const float v) { progressed_files = v; }
    void setRecipients(const std::vector<QString>& v);

public slots:
    void setStatusText(QString result) {
        status_text += result;
        emit statusTextChanged();
    }

signals:
    void emailChanged();
    void idChanged();
    void passwordEntropyChanged();
    void recipientsChanged();
    void progressedFilesChanged(float);
    void processingFinishedChanged(bool);
    void statusTextChanged();

private:
    minilockcpp::minilock session;
    std::vector<std::string> recipients;
    QThread *thread;
    QString status_text;
    float progressed_files = 0.0;
    float password_entropy = 0.0;
    bool processing_finished = false;
};
