#ifndef APPLICATIONFILE2_H
#define APPLICATIONFILE2_H
#include "applicationfilemodel.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QUrl>
#include <machinetalk/protobuf/config.pb.h>
#include <machinetalk/protobuf/message.pb.h>
#include <remotefile/filebase.h>

namespace qtquickvcp {

class ApplicationFile2 : public machinetalk::remotefile::FileBase {
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString remoteFilePath READ remoteFilePath WRITE setRemoteFilePath NOTIFY remoteFilePathChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString serverDirectory READ serverDirectory WRITE setServerDirectory NOTIFY serverDirectoryChanged)
    Q_PROPERTY(TransferState transferState READ transferState NOTIFY transferStateChanged)
    Q_PROPERTY(TransferError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(ApplicationFileModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)
    Q_ENUMS(TransferState TransferError)

public:
    explicit ApplicationFile2(QObject* parent = nullptr);
    ~ApplicationFile2();

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
        ServiceError = 1,
        FileError = 2
    };

    bool isConnected() const
    {
        return m_connected;
    }

    QString remoteFilePath() const
    {
        return m_remoteFilePath;
    }

    QString localFilePath() const
    {
        return m_localFilePath;
    }

    QString remotePath() const
    {
        return m_remotePath;
    }

    QString localPath() const
    {
        return m_localPath;
    }

    QString serverDirectory() const
    {
        return m_serverDirectory;
    }

    TransferState transferState() const
    {
        return m_transferState;
    }

    TransferError error() const
    {
        return m_error;
    }

    ApplicationFileModel* model() const
    {
        return m_model;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    double progress() const
    {
        return m_progress;
    }

    bool showHidden() const
    {
        return m_showHidden;
    }

public slots:
    void setRemoteFilePath(QString remoteFilePath)
    {
        if (m_remoteFilePath == remoteFilePath)
            return;

        m_remoteFilePath = remoteFilePath;
        emit remoteFilePathChanged(remoteFilePath);
    }

    void setLocalFilePath(QString localFilePath)
    {
        if (m_localFilePath == localFilePath)
            return;

        m_localFilePath = localFilePath;
        emit localFilePathChanged(localFilePath);
    }

    void setRemotePath(QString remotePath)
    {
        if (m_remotePath == remotePath)
            return;

        m_remotePath = remotePath;
        emit remotePathChanged(remotePath);
    }

    void setLocalPath(QString localPath)
    {
        if (m_localPath == localPath)
            return;

        m_localPath = localPath;
        emit localPathChanged(localPath);
    }

    void setServerDirectory(QString serverDirectory)
    {
        if (m_serverDirectory == serverDirectory)
            return;

        m_serverDirectory = serverDirectory;
        emit serverDirectoryChanged(serverDirectory);
    }

    void setShowHidden(bool showHidden)
    {
        if (m_showHidden == showHidden)
            return;

        m_showHidden = showHidden;
        emit showHiddenChanged(m_showHidden);
    }

    void startUpload();
    void startDownload();
    void refreshFiles();
    void removeFile(const QString& name);
    void createDirectory(const QString& name);
    void abort();
    void clearError();
    void cleanupFile();

private slots:
    void handleFileListingMessage(const machinetalk::Container& rx);
    void handleFileDataMessage(const machinetalk::Container& rx);
    void setConnected();
    void clearConnected();
    void getCmdStarted();
    void getCmdSucceded();
    void getCmdFailed();
    void putCmdStarted();
    void putCmdSucceded();
    void putCmdFailed();
    void lsCmdStarted();
    void lsCmdSucceded();
    void lsCmdFailed();
    void mkdirCmdStarted();
    void mkdirCmdSucceded();
    void mkdirCmdFailed();
    void deleteCmdStarted();
    void deleteCmdSucceded();
    void deleteCmdFailed();

signals:
    void uploadFinished();
    void downloadFinished();
    void refreshFinished();
    void removeFinished();
    void createDirectoryFinished();
    void connectedChanged(bool connected);
    void remoteFilePathChanged(QString remoteFilePath);
    void localFilePathChanged(QString localFilePath);
    void remotePathChanged(QString remotePath);
    void localPathChanged(QString localPath);
    void serverDirectoryChanged(QString serverDirectory);
    void transferStateChanged(TransferState transferState);
    void errorChanged(TransferError error);
    void modelChanged(ApplicationFileModel* model);

    void errorStringChanged(QString errorString);

    void progressChanged(double progress);

    void showHiddenChanged(bool showHidden);

private:
    bool m_connected;
    QString m_remoteFilePath;
    QString m_localFilePath;
    QString m_remotePath;
    QString m_localPath;
    QString m_serverDirectory;
    TransferState m_transferState;
    TransferError m_error;
    ApplicationFileModel* m_model;

    QFile* m_file;

    // more efficient to reuse a protobuf Message
    machinetalk::Container m_tx;

    QString generateTempPath() const;
    void cleanupTempPath();
    QString applicationFilePath(const QString& remoteFilePath, const QString& serverDirectory) const;

    void commandSucceded();
    void commandFailed();
    void updateState(TransferState state);
    void updateError(TransferError error, const QString& errorString = QLatin1String(""));

    QString m_errorString;
    double m_progress;
    bool m_showHidden;
}; // class ApplicationFile2

} // namespace qtquickvcp

#endif // APPLICATIONFILE2_H
