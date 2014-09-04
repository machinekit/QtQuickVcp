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
    m_file(NULL)
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

    if (!m_ready || (m_transferState != NoTransfer))
    {
        return;
    }

    url.setUrl(m_uri + "/" + fileInfo.fileName());
    m_fileName = fileInfo.fileName();
    emit fileNameChanged(m_fileName);

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
    QString filePath;

    if (!m_ready || (m_transferState != NoTransfer))
    {
        return;
    }

    url.setUrl(m_uri + "/" + m_fileName);

    filePath = applicationFilePath(m_fileName);
    m_localFilePath = QUrl::fromLocalFile(filePath).toString();
    emit localFilePathChanged(m_localFilePath);

    QFileInfo fileInfo(filePath);

    if (!dir.mkpath(fileInfo.absolutePath()))
    {
        updateState(Error);
        updateError(FileError, tr("Cannot create directory"));
        return;
    }

    m_file = new QFile(filePath);

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
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
}

QString QApplicationFile::applicationFilePath(const QString &fileName)
{
    return QString("%1/machinekit-%2/%3").arg(QDir::tempPath())
            .arg(QCoreApplication::applicationPid())
            .arg(fileName);
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
