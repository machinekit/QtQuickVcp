#include "qapplicationfile.h"

QApplicationFile::QApplicationFile(QQuickItem *parent) :
    QQuickItem(parent),
    m_uri(""),
    m_ready(false),
    m_transferState(NoTransfer),
    m_error(NoError),
    m_errorString(""),
    m_localFilePath(""),
    m_progress(0.0),
    m_componentCompleted(false),
    m_networkManager(NULL),
    m_reply(NULL),
    m_uploadFile(NULL)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

QApplicationFile::~QApplicationFile()
{
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

    if (!m_ready)
    {
        return;
    }

    url.setUrl(m_uri + "/" + fileInfo.fileName());
    m_fileName = fileInfo.fileName();
    emit fileNameChanged(m_fileName);

    m_uploadFile = new QFile(fileInfo.filePath());

    if (m_uploadFile->open(QIODevice::ReadOnly))
    {
        m_reply = m_networkManager->put(QNetworkRequest(url), m_uploadFile);
        connect(m_reply, SIGNAL(uploadProgress(qint64, qint64)),
                this,SLOT(uploadProgress(qint64, qint64)));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
        updateState(UploadRunning);
        updateError(NoError, "");
    }
    else
    {
        updateState(Error);
        updateError(FileError, tr("Cannot open file"));
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
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

void QApplicationFile::replyFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply)

    m_reply->deleteLater();
    m_uploadFile->close();
    m_uploadFile->deleteLater();
    m_reply = NULL;
    m_uploadFile = NULL;
    m_networkManager->clearAccessCache();

    updateState(NoTransfer);

    if (m_error == NoError)
    {
        emit uploadFinished();
    }
}

void QApplicationFile::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
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
