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


#ifndef _MINILOCKCPP_GUI_QMINILOCKWORKER_H_
#define _MINILOCKCPP_GUI_QMINILOCKWORKER_H_


#include <QObject>
#include <QThread>

#include <minilock.h>

class QMinilockWorker : public QObject
{
    Q_OBJECT

public:
    QMinilockWorker(minilockcpp::minilock* session, QList<QString> fileNames, 
        bool randomFilename,
        QString sourceDirectory, QString targetDirectory,
        std::vector<std::string> recipients);

    QMinilockWorker(minilockcpp::minilock* session, QList<QString> fileNames, 
        QString sourceDirectory, QString targetDirectory);


public slots:
    void encryptFiles();
    void decryptFiles();


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


#endif // _MINILOCKCPP_GUI_QMINILOCKWORKER_H_