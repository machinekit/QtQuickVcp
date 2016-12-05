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
#ifndef APPLICATIONFILE_H
#define APPLICATIONFILE_H

#include <abstractserviceimplementation.h>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QDir>
#include "qftp.h"
#include "applicationfilemodel.h"

namespace qtquickvcp {

class ApplicationFile : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString remoteFilePath READ remoteFilePath WRITE setRemoteFilePath NOTIFY remoteFilePathChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString serverDirectory READ serverDirectory WRITE setServerDirectory NOTIFY serverDirectoryChanged)
    Q_PROPERTY(TransferState transferState READ transferState NOTIFY transferStateChanged)
    Q_PROPERTY(TransferError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(bool networkReady READ networkReady NOTIFY networkReadyChanged)
    Q_PROPERTY(bool editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    Q_PROPERTY(ApplicationFileModel *model READ model NOTIFY modelChanged)
    Q_ENUMS(TransferState TransferError)

public:
    explicit ApplicationFile(QObject *parent = 0);
    ~ApplicationFile();

    enum TransferState {
        NoTransfer = 0,
        UploadRunning = 1,
        DownloadRunning = 2,
        RefreshRunning = 3,
        RemoveRunning = 4,
        RemoveDirectoryRunning = 5,
        CreateDirectoryRunning = 6,
        Error = 7
    };

    enum TransferError {
        NoError = 0,
        FtpError = 1,
        FileError = 2
    };

    bool editMode() const {
        return m_editMode;
    }

    QString uri() const
    {
        return m_uri;
    }

    TransferError error() const
    {
        return m_error;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    QString localFilePath() const
    {
        return m_localFilePath;
    }

    QString remoteFilePath() const
    {
        return m_remoteFilePath;
    }

    QString localPath() const
    {
        return m_localPath;
    }

    QString remotePath() const
    {
        return m_remotePath;
    }

    double progress() const
    {
        return m_progress;
    }

    TransferState transferState() const
    {
        return m_transferState;
    }

    bool networkReady() const
    {
        return m_networkReady;
    }

    ApplicationFileModel * model() const
    {
        return m_model;
    }

    QString serverDirectory() const
    {
        return m_serverDirectory;
    }

public slots:
    void setEditMode(const bool &a) {
        if (a != m_editMode) {
            m_editMode = a;
            emit editModeChanged();
        }
    }

    void setUri(QString arg)
    {
        if (m_uri == arg)
            return;

        m_uri = arg;
        emit uriChanged(arg);
    }

    void setLocalFilePath(QString arg)
    {
        if (m_localFilePath == arg)
            return;

        m_localFilePath = arg;
        emit localFilePathChanged(arg);
    }

    void setRemoteFilePath(QString arg)
    {
        if (m_remoteFilePath == arg)
            return;

        m_remoteFilePath = arg;
        emit remoteFilePathChanged(arg);
    }

    void setLocalPath(QString arg)
    {
        if (m_localPath == arg)
            return;

        m_localPath = arg;
        emit localPathChanged(arg);
    }

    void setRemotePath(QString arg)
    {
        if (m_remotePath == arg)
            return;

        m_remotePath = arg;
        emit remotePathChanged(arg);
    }

    void setServerDirectory(QString serverDirectory)
    {
        if (m_serverDirectory == serverDirectory)
            return;

        m_serverDirectory = serverDirectory;
        emit serverDirectoryChanged(serverDirectory);
    }

    void startUpload();
    void startDownload();
    void refreshFiles();
    void removeFile(const QString &name);
    void removeDirectory(const QString &name);
    void createDirectory(const QString &name);
    void abort();
    void clearError();

private:
    bool            m_editMode;
    QString         m_uri;
    QString         m_localFilePath;
    QString         m_remoteFilePath;
    QString         m_localPath;
    QString         m_remotePath;
    QString         m_serverDirectory;
    TransferState   m_transferState;
    TransferError   m_error;
    QString         m_errorString;
    double          m_progress;
    bool            m_networkReady;
    ApplicationFileModel * m_model;

    QNetworkAccessManager   *m_networkManager;
    QFile                   *m_file;
    QFtp                    *m_ftp;

    void start() {}
    void stop() {}
    void updateState(TransferState state);
    void updateError(TransferError error, const QString &errorString);
    QString generateTempPath();
    void cleanupTempPath();
    QString applicationFilePath(const QString &remoteFilePath, const QString &serverDirectory);
    void initializeFtp();
    void cleanupFtp();
    void cleanupFile();

private slots:
    void transferProgress(qint64 bytesSent, qint64 bytesTotal);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accesible);
    void addToList(const QUrlInfo &urlInfo);
    void ftpCommandFinished(int, bool error);

signals:
    void editModeChanged();
    void uriChanged(QString arg);
    void errorChanged(TransferError arg);
    void errorStringChanged(QString arg);
    void localFilePathChanged(QString arg);
    void remoteFilePathChanged(QString arg);
    void localPathChanged(QString arg);
    void remotePathChanged(QString arg);
    void serverDirectoryChanged(QString serverDirectory);
    void progressChanged(double arg);
    void transferStateChanged(TransferState arg);
    void uploadFinished();
    void downloadFinished();
    void refreshFinished();
    void removeFinished();
    void removeDirectoryFinished();
    void createDirectoryFinished();
    void networkReadyChanged(bool networkReady);
    void modelChanged(ApplicationFileModel * model);
}; // class ApplicationFile
}; // namespace qtquickvcp

#endif // APPLICATIONFILE_H
