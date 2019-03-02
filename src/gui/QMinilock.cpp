#include <exception>
#include <cmath>

#include "QMinilock.h"
#include "QMinilockWorker.h"

template <typename T> static float ShannonEntropy(T data[],int elements){
    float entropy=0;
    std::map<T,long> counts;
    typename std::map<T,long>::iterator it;
    for (int dataIndex = 0; dataIndex < elements; ++dataIndex) {
        counts[data[dataIndex]]++;
    }
    it = counts.begin();
    while(it != counts.end()){
        float p_x = (float)it->second/elements;
        if (p_x>0) entropy-=p_x*log(p_x)/log(2);
        it++;
    }
    return entropy;
}

void QMinilock::setPasswordEntropy(QString password) {
    std::string p = password.toStdString();
    password_entropy = ShannonEntropy(password.data(), password.size()) * password.size();
    emit passwordEntropyChanged();
}

void QMinilock::setRecipients(const std::vector<QString>& v)
{
    recipients.clear();
    for(auto recipient : v)
    {
        if(!recipient.toStdString().empty())
        {
            recipients.push_back(recipient.toStdString());
        }
    }
}

void QMinilock::cancleAll() {
    if(thread->isRunning())
    {
        thread->requestInterruption();
    }
}



bool QMinilock::initialize(QString email, QString password)
{
    if(session.initialize(email.toStdString(), password.toStdString()))
    {
        emit emailChanged();
        emit idChanged();
        return true;
    }
    return false;
}

void QMinilock::logout() {
    cancleAll();
    clearStatusText();
}

void QMinilock::encryptFiles(QList<QString> fileNames, bool random_filename, QString sourceDirectory, QString targetDirectory)
{
    progressed_files = 0;
    status_text = "";
    emit statusTextChanged();
    emit progressedFilesChanged(progressed_files);

    thread = new QThread;
    Worker* worker = new Worker(&session, fileNames, random_filename, sourceDirectory, targetDirectory, recipients);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(encryptFiles()));
    connect(worker, SIGNAL(statusTextChanged(QString)), this, SLOT(setStatusText(QString)));
    connect(worker, &Worker::progressedFileFinished, this, [this]() {
        progressed_files++;
        emit progressedFilesChanged(progressed_files);
    });
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(destroyed()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    thread->start();
}


void QMinilock::decryptFiles(QList<QString> fileNames, QString sourceDirectory, QString targetDirectory)
{
    progressed_files = 0;
    status_text = "";
    emit statusTextChanged();
    emit progressedFilesChanged(progressed_files);

    thread = new QThread;
    Worker* worker = new Worker(&session, fileNames, sourceDirectory, targetDirectory);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(decryptFiles()));
    connect(worker, SIGNAL(statusTextChanged(QString)), this, SLOT(setStatusText(QString)));
    connect(worker, &Worker::progressedFileFinished, this, [this]() {
        progressed_files++;
        emit progressedFilesChanged(progressed_files);
    });
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(destroyed()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    thread->start();
}
