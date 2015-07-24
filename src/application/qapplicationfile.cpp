/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

#include "qapplicationfile.h"

QApplicationFile::QApplicationFile(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_uri(""),
    m_localFilePath(""),
    m_remoteFilePath(""),
    m_localPath(""),
    m_remotePath(""),
    m_transferState(NoTransfer),
    m_error(NoError),
    m_errorString(""),
    m_progress(0.0),
    m_networkReady(false),
    m_model(NULL),
    m_networkManager(NULL),
    m_file(NULL),
    m_ftp(NULL)
{
    m_localPath = generateTempPath();

    m_model = new QApplicationFileModel(this);

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));

    if (m_networkManager->networkAccessible() == QNetworkAccessManager::Accessible)
    {
        initializeFtp();
    }
}

QApplicationFile::~QApplicationFile()
{
    cleanupTempPath();
    cleanupFtp();
    m_networkManager->deleteLater();
    m_model->deleteLater();
}

void QApplicationFile::startUpload()
{
    QUrl url;
    QFileInfo fileInfo(QUrl(m_localFilePath).toLocalFile());
    QString remotePath;

    if (!ready() || (m_transferState != NoTransfer))
    {
        return;
    }

    url.setUrl(m_uri + "/" + fileInfo.fileName());
    remotePath = QUrl(m_remotePath).toLocalFile();
    m_remoteFilePath = QUrl::fromLocalFile(QDir(remotePath).filePath(fileInfo.fileName())).toString();
    emit remoteFilePathChanged(m_remoteFilePath);

    m_file = new QFile(fileInfo.filePath());

    if (m_file->open(QIODevice::ReadOnly))
    {
        m_ftp->connectToHost(url.host(), url.port());
        m_ftp->login();
        m_ftp->put(m_file, fileInfo.fileName(), QFtp::Binary);
        m_ftp->close();

        m_progress = 0.0;
        emit progressChanged(m_progress);
        updateState(UploadRunning);
        updateError(NoError, "");
    }
    else
    {
        updateState(Error);
        updateError(FileError, m_file->errorString());
    }
}

void QApplicationFile::startDownload()
{
    QUrl url;
    QDir dir;
    QString localFilePath;
    QString remoteFilePath;
    QString remotePath;
    QString fileName;

    if (!ready() || (m_transferState != NoTransfer))
    {
        return;
    }

    remoteFilePath = QUrl(m_remoteFilePath).toLocalFile();
    remotePath = QUrl(m_remotePath).toLocalFile();
    fileName = remoteFilePath.mid(remotePath.length() + 1);

    url.setUrl(m_uri + "/" + fileName);

    localFilePath = applicationFilePath(fileName);
    m_localFilePath = QUrl::fromLocalFile(localFilePath).toString();
    emit localFilePathChanged(m_localFilePath);

    QFileInfo fileInfo(localFilePath);

    if (!dir.mkpath(fileInfo.absolutePath()))
    {
        updateState(Error);
        updateError(FileError, tr("Cannot create directory"));
        return;
    }

    m_file = new QFile(localFilePath);

    if (m_file->open(QIODevice::WriteOnly))
    {
        m_ftp->connectToHost(url.host(), url.port());
        m_ftp->login();
        m_ftp->get(fileName, m_file, QFtp::Binary);
        m_ftp->close();
        m_progress = 0.0;
        emit progressChanged(m_progress);
        updateState(DownloadRunning);
        updateError(NoError, "");
    }
    else
    {
        updateState(Error);
        updateError(FileError, m_file->errorString());
    }
}

void QApplicationFile::refreshFiles()
{
    QUrl url(m_uri);

    if (!ready() || (m_transferState != NoTransfer))
    {
        return;
    }

    m_model->clear();
    m_ftp->connectToHost(url.host(), url.port());
    m_ftp->login();
    m_ftp->list();
    m_ftp->close();
    updateState(RefreshRunning);
}

void QApplicationFile::removeFile(QString name)
{
    QUrl url(m_uri);

    if (!ready() || (m_transferState != NoTransfer))
    {
        return;
    }

    m_ftp->connectToHost(url.host(), url.port());
    m_ftp->login();
    m_ftp->remove(name);
    m_ftp->close();
    updateState(RemoveRunning);
}

void QApplicationFile::abort()
{
    if (!m_ftp)
    {
        return;
    }
    updateState(NoTransfer);
    m_ftp->abort();
    m_ftp->close();
}

void QApplicationFile::clearError()
{
    updateState(NoTransfer);
    updateError(NoError, "");
}

void QApplicationFile::updateState(QApplicationFile::TransferState state)
{
    if (state != m_transferState)
    {
        m_transferState = state;
        emit transferStateChanged(m_transferState);
    }
}

void QApplicationFile::updateError(QApplicationFile::TransferError error, const QString &errorString)
{
    if (m_errorString != errorString)
    {
        m_errorString = errorString;
        emit errorStringChanged(m_errorString);
    }

    if (m_error != error)
    {
        m_error = error;
        emit errorChanged(m_error);
    }
}

QString QApplicationFile::generateTempPath()
{
    return QUrl::fromLocalFile(QString("%1/machinekit-%2").arg(QDir::tempPath())
            .arg(QCoreApplication::applicationPid())).toString();
}

void QApplicationFile::cleanupTempPath()
{
    QDir dir(m_localPath);

    dir.removeRecursively();
}

QString QApplicationFile::applicationFilePath(const QString &fileName)
{
    return QDir(QUrl(m_localPath).toLocalFile()).filePath(fileName);
}

void QApplicationFile::initializeFtp()
{
    m_ftp = new QFtp(this);
    connect(m_ftp, SIGNAL(commandFinished(int,bool)),
    this, SLOT(ftpCommandFinished(int,bool)));
    connect(m_ftp, SIGNAL(listInfo(QUrlInfo)),
    this, SLOT(addToList(QUrlInfo)));
    connect(m_ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
    this, SLOT(transferProgress(qint64,qint64)));

    m_networkReady = true;
    emit readyChanged(m_networkReady);
}

void QApplicationFile::cleanupFtp()
{
    if (m_ftp == NULL)
    {
        return;
    }

    m_ftp->abort();
    m_ftp->deleteLater();
    m_ftp = NULL;

    m_networkReady = false;
    emit readyChanged(m_networkReady);
}

void QApplicationFile::transferProgress(qint64 bytesSent, qint64 bytesTotal)
{
    m_progress = (double)bytesSent / (double)bytesTotal;
    emit progressChanged(m_progress);
}

void QApplicationFile::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accesible)
{
    if (accesible == QNetworkAccessManager::Accessible)
    {
        initializeFtp();
    }
    else
    {
        cleanupFtp();
    }
}

void QApplicationFile::addToList(const QUrlInfo &urlInfo)
{
    QApplicationFileItem *item;

    if (!urlInfo.isDir()) // for now disable folders
    {
        item = new QApplicationFileItem();
        item->setName(urlInfo.name());
        item->setSize(urlInfo.size());
        item->setOwner(urlInfo.owner());
        item->setGroup(urlInfo.group());
        item->setLastModified(urlInfo.lastModified());
        item->setDir(urlInfo.isDir());

        m_model->addItem(item);
    }
}

void QApplicationFile::ftpCommandFinished(int, bool error)
{
    if (error)
    {
        if (m_file)
        {
            m_file->close();
            m_file->deleteLater();
            m_file = NULL;
        }
        m_ftp->close();

        if (m_transferState != NoTransfer) // may be a user abort operation
        {
            updateState(Error);
            updateError(FtpError, m_ftp->errorString());
        }

        return;
    }

    if (m_ftp->currentCommand() == QFtp::Get)
    {
        m_file->close();
        m_file->deleteLater();
        m_file = NULL;
        emit downloadFinished();
        updateState(NoTransfer);

        return;
    }

    if (m_ftp->currentCommand() == QFtp::Put)
    {
        m_file->close();
        m_file->deleteLater();
        m_file = NULL;
        emit uploadFinished();
        updateState(NoTransfer);
        refreshFiles();

        return;
    }

    if (m_ftp->currentCommand() == QFtp::List)
    {
        emit refreshFinished();
        updateState(NoTransfer);

        return;
    }

    if (m_ftp->currentCommand() == QFtp::Remove)
    {
        emit removeFinished();
        updateState(NoTransfer);
        refreshFiles();
    }
}
