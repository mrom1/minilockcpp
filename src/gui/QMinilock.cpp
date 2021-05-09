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

QMinilock::QMinilock(QObject* parent /* = nullptr */)
    : QObject(parent),
      thread(nullptr)
{
}


QMinilock::~QMinilock()
{
    if (thread != nullptr)
    {
        thread->quit();
        thread->wait();
    }
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

void QMinilock::cancleAll() 
{
    if(thread != nullptr && thread->isRunning())
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

    QMinilockWorker* worker = new QMinilockWorker(&session, fileNames, random_filename, sourceDirectory, targetDirectory, recipients);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(encryptFiles()));
    connect(worker, SIGNAL(statusTextChanged(QString)), this, SLOT(setStatusText(QString)));
    connect(worker, &QMinilockWorker::progressedFileFinished, this, [this]() 
    {
        progressed_files++;
        emit progressedFilesChanged(progressed_files);
    });    
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

    QMinilockWorker* worker = new QMinilockWorker(&session, fileNames, sourceDirectory, targetDirectory);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(decryptFiles()));
    connect(worker, SIGNAL(statusTextChanged(QString)), this, SLOT(setStatusText(QString)));
    connect(worker, &QMinilockWorker::progressedFileFinished, this, [this]() 
    {
        progressed_files++;
        emit progressedFilesChanged(progressed_files);
    });    
    connect(worker, SIGNAL(destroyed()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    thread->start();
}
