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



#include "QMinilockWorker.h"


QMinilockWorker::QMinilockWorker(minilockcpp::minilock* session, QList<QString> fileNames, bool randomFilename,
    QString sourceDirectory, QString targetDirectory,
    std::vector<std::string> recipients)
    : session(session),
    fileNames(fileNames),
    randomFilename(randomFilename),
    sourceDirectory(sourceDirectory),
    targetDirectory(targetDirectory),
    recipients(recipients)
{
}



QMinilockWorker::QMinilockWorker(minilockcpp::minilock* session, QList<QString> fileNames, QString sourceDirectory, QString targetDirectory)
    : session(session),
    fileNames(fileNames),
    sourceDirectory(sourceDirectory),
    targetDirectory(targetDirectory),
    randomFilename(false)
{
}


void QMinilockWorker::encryptFiles()
{
    for (auto file : fileNames) 
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            return;
        }
        
        QString status = "Encrypting " + file + " ... ";
        
        emit statusTextChanged(status);

        try 
        {
            if (session->encrypt_file(file.toStdString(), randomFilename, sourceDirectory.toStdString(), targetDirectory.toStdString(), recipients))
            {
                status = "OK!\n";
                emit statusTextChanged(status);
                emit progressedFileFinished();
            }
        }
        catch (std::exception& e) 
        {
            status = "FAIL!\n[->] ";
            status += e.what();
            status += "\n";
            emit statusTextChanged(status);
        }
    }
}


void QMinilockWorker::decryptFiles()
{

    for (auto file : fileNames) 
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            return;
        }

        QString status = "Decrypting " + file + " ... ";

        emit statusTextChanged(status);

        try 
        {
            if (session->decrypt_file(file.toStdString(), sourceDirectory.toStdString(), targetDirectory.toStdString()))
            {
                status = "OK!\n";
                emit statusTextChanged(status);
                emit progressedFileFinished();
            }
        }
        catch (std::exception& e) 
        {
            status = "FAIL!\n[->] ";
            status += e.what();
            status += "\n";
            emit statusTextChanged(status);
        }
    }
}