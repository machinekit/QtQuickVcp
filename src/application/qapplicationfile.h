#ifndef QAPPLICATIONFILE_H
#define QAPPLICATIONFILE_H

#include <abstractserviceimplementation.h>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QDir>

class QApplicationFile : public AbstractServiceImplementation
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString remoteFilePath READ remoteFilePath WRITE setRemoteFilePath NOTIFY remoteFilePathChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(TransferState transferState READ transferState NOTIFY transferStateChanged)
    Q_PROPERTY(TransferError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_ENUMS(TransferState TransferError)

public:
    explicit QApplicationFile(QObject *parent = 0);
    ~QApplicationFile();

    enum TransferState {
        NoTransfer = 0,
        UploadRunning = 1,
        DownloadRunning = 2,
        Error = 3
    };

    enum TransferError {
        NoError = 0,
        FtpError = 1,
        FileError = 2
    };

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

public slots:
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

    void startUpload();
    void startDownload();
    void abort();

private:
    QString         m_uri;
    QString         m_localFilePath;
    QString         m_remoteFilePath;
    QString         m_localPath;
    QString         m_remotePath;
    TransferState   m_transferState;
    TransferError   m_error;
    QString         m_errorString;
    double          m_progress;

    QNetworkAccessManager   *m_networkManager;
    QNetworkReply           *m_reply;
    QFile                   *m_file;

    void start() {}
    void stop() {}
    void updateState(TransferState state);
    void updateError(TransferError error, const QString &errorString);
    QString generateTempPath();
    void cleanupTempPath();
    QString applicationFilePath(const QString &remoteFilePath);

private slots:
    void readyRead();
    void replyFinished(QNetworkReply * reply);
    void transferProgress(qint64 bytesSent, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);

signals:
    void uriChanged(QString arg);
    void errorChanged(TransferError arg);
    void errorStringChanged(QString arg);
    void localFilePathChanged(QString arg);
    void remoteFilePathChanged(QString arg);
    void localPathChanged(QString arg);
    void remotePathChanged(QString arg);
    void progressChanged(double arg);
    void transferStateChanged(TransferState arg);
    void uploadFinished();
    void downloadFinished();
};

#endif // QAPPLICATIONFILE_H
