#ifndef QAPPLICATIONFILE_H
#define QAPPLICATIONFILE_H

#include <QQuickItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>

class QApplicationFile : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(TransferState transferState READ transferState NOTIFY transferStateChanged)
    Q_PROPERTY(TransferError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_ENUMS(TransferState TransferError)

public:
    explicit QApplicationFile(QQuickItem *parent = 0);
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

    virtual void componentComplete();

    QString uri() const
    {
        return m_uri;
    }

    bool ready() const
    {
        return m_ready;
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

    double progress() const
    {
        return m_progress;
    }

    TransferState transferState() const
    {
        return m_transferState;
    }

    QString fileName() const
    {
        return m_fileName;
    }

public slots:
    void setUri(QString arg)
    {
        if (m_uri == arg)
            return;

        m_uri = arg;
        emit uriChanged(arg);
    }

    void setReady(bool arg)
    {
        if (m_ready == arg)
            return;

        m_ready = arg;
        emit readyChanged(arg);
    }

    void setLocalFilePath(QString arg)
    {
        if (m_localFilePath == arg)
            return;

        m_localFilePath = arg;
        emit localFilePathChanged(arg);
    }

    void startUpload();
    void abort();

private:
    QString         m_uri;
    QString         m_fileName;
    bool            m_ready;
    TransferState   m_transferState;
    TransferError   m_error;
    QString         m_errorString;
    QString         m_localFilePath;
    double          m_progress;
    bool            m_componentCompleted;

    QNetworkAccessManager   *m_networkManager;
    QNetworkReply           *m_reply;
    QFile                   *m_uploadFile;

    void updateState(TransferState state);
    void updateError(TransferError error, const QString &errorString);

private slots:
    void replyFinished(QNetworkReply * reply);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);

signals:
    void uriChanged(QString arg);
    void readyChanged(bool arg);
    void errorChanged(TransferError arg);
    void errorStringChanged(QString arg);
    void localFilePathChanged(QString arg);
    void progressChanged(double arg);
    void transferStateChanged(TransferState arg);
    void uploadFinished();
    void fileNameChanged(QString arg);
};

#endif // QAPPLICATIONFILE_H
