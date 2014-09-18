#include "qapplicationfile.h"

QApplicationFile::QApplicationFile(QQuickItem *parent) :
    QQuickItem(parent),
    m_uri(""),
    m_localFilePath(""),
    m_remoteFilePath(""),
    m_localPath(""),
    m_remotePath(""),
    m_ready(false),
    m_transferState(NoTransfer),
    m_error(NoError),
    m_errorString(""),
    m_progress(0.0),
    m_componentCompleted(false),
    m_networkManager(NULL),
    m_reply(NULL),
    m_file(NULL)
{
    m_localPath = generateTempPath();

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

QApplicationFile::~QApplicationFile()
{
    cleanupTempPath();
    m_networkManager->deleteLater();
}

/** componentComplete is executed when the QML component is fully loaded */
void QApplicationFile::componentComplete()
{
    m_componentCompleted = true;

    QQuickItem::componentComplete();
}

void QApplicationFile::startUpload()
{
    QUrl url;
    QFileInfo fileInfo(QUrl(m_localFilePath).toLocalFile());
    QString remotePath;

    if (!m_ready || (m_transferState != NoTransfer))
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
        m_reply = m_networkManager->put(QNetworkRequest(url), m_file);
        connect(m_reply, SIGNAL(uploadProgress(qint64,qint64)),
                this,SLOT(transferProgress(qint64, qint64)));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
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

    if (!m_ready || (m_transferState != NoTransfer))
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
        m_reply = m_networkManager->get(QNetworkRequest(url));
        connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(transferProgress(qint64, qint64)));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
        connect(m_reply, SIGNAL(readyRead()),
                this, SLOT(readyRead()));
        updateState(DownloadRunning);
        updateError(NoError, "");
    }
    else
    {
        updateState(Error);
        updateError(FileError, m_file->errorString());
    }
}

void QApplicationFile::abort()
{
    if (!m_reply)
    {
        return;
    }
    m_reply->abort();
    updateState(NoTransfer);
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

void QApplicationFile::readyRead()
{
    m_file->write(m_reply->readAll());
}

void QApplicationFile::replyFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply)

    m_reply->deleteLater();
    m_file->close();
    m_file->deleteLater();
    m_reply = NULL;
    m_file = NULL;
    m_networkManager->clearAccessCache();


    if (m_error == NoError)
    {
        if (m_transferState == UploadRunning) {
            emit uploadFinished();
        }
        else {
            emit downloadFinished();
        }
    }

    updateState(NoTransfer);
}

void QApplicationFile::transferProgress(qint64 bytesSent, qint64 bytesTotal)
{
    m_progress = (double)bytesSent / (double)bytesTotal;
    emit progressChanged(m_progress);
}

void QApplicationFile::error(QNetworkReply::NetworkError code)
{
    if (code == QNetworkReply::OperationCanceledError) // ignore user cancel
    {
        return;
    }

    updateState(Error);
    updateError(FtpError, m_reply->errorString());
}
