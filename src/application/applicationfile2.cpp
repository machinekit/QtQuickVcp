#include "applicationfile2.h"

namespace qtquickvcp {

ApplicationFile2::ApplicationFile2(QObject *parent):
    machinetalk::remotefile::FileBase(parent),
    m_connected(false),
    m_remoteFilePath(""),
    m_localFilePath(""),
    m_remotePath(""),
    m_localPath(""),
    m_serverDirectory(""),
    m_transferState(NoTransfer),
    m_error(NoError),
    m_file(nullptr)
{
    m_localPath = generateTempPath();

    m_model = new ApplicationFileModel(this);
}

ApplicationFile2::~ApplicationFile2()
{
    cleanupTempPath();
    cleanupFile();
    m_model->deleteLater();
}

void ApplicationFile2::startUpload()
{
    QFileInfo fileInfo(QUrl(m_localFilePath).toLocalFile());
    QString remotePath;
    QString remoteFilePath;
    QString uploadFilePath;

    if (!m_connected || (m_transferState != NoTransfer))
    {
        return;
    }

    // create the remote path to send to service
    remotePath = "";
    if (!m_serverDirectory.isEmpty())
    {
        remotePath.append(m_serverDirectory);
    }
    uploadFilePath = QDir(remotePath).filePath(fileInfo.fileName());

    // extract remote path url from file
    remotePath = QUrl(m_remotePath).toLocalFile();
    remoteFilePath = QDir::cleanPath(remotePath + "/" + uploadFilePath);
    m_remoteFilePath = QUrl::fromLocalFile(remoteFilePath).toString();
    emit remoteFilePathChanged(m_remoteFilePath);

    m_file = new QFile(fileInfo.filePath(), this);
    if (m_file->open(QIODevice::ReadOnly))
    {
        machinetalk::File *fileEntry =  m_tx.mutable_file_service_data()->add_files();

        fileEntry->set_name(uploadFilePath.toStdString());
        fileEntry->set_blob(m_file->readAll().toStdString());
        this->sendFilePut(m_tx);

        updateError(NoError);
    }
    else
    {
        updateState(Error);
        updateError(FileError); // TODO: , m_file->errorString()
    }
}

void ApplicationFile2::startDownload()
{
    QDir dir;
    QString localFilePath;
    QString remoteFilePath;
    QString remotePath;
    QString fileName;

    if (!m_connected || (m_transferState != NoTransfer))
    {
        return;
    }

    remoteFilePath = QUrl(m_remoteFilePath).toLocalFile();
    remotePath = QUrl(m_remotePath).toLocalFile();
    fileName = remoteFilePath.mid(remotePath.length() + 1);

    int i = fileName.indexOf("/");
    if (i > -1) // not in root directory
    {
        m_serverDirectory = fileName.left(i);
        emit serverDirectoryChanged(m_serverDirectory);
        fileName = fileName.mid(i + 1);
    }

    localFilePath = applicationFilePath(fileName, m_serverDirectory);
    m_localFilePath = QUrl::fromLocalFile(localFilePath).toString();
    emit localFilePathChanged(m_localFilePath);

    QFileInfo fileInfo(localFilePath);

    if (!dir.mkpath(fileInfo.absolutePath()))
    {
        updateState(Error);
        updateError(FileError); // TODO: tr("Cannot create directory: %1").arg(fileInfo.absolutePath())
        return;
    }

    m_file = new QFile(localFilePath, this);
    if (m_file->open(QIODevice::WriteOnly))
    {
        machinetalk::File *fileEntry =  m_tx.mutable_file_service_data()->add_files();
        fileEntry->set_name(QString(m_serverDirectory + "/" + fileName).toStdString());
        this->sendFileGet(m_tx);

        updateError(NoError);
    }
    else
    {
        updateState(Error);
        updateError(FileError); // TODO: , m_file->errorString()
    }
}

void ApplicationFile2::refreshFiles()
{
    if (!m_connected || (m_transferState != NoTransfer))
    {
        return;
    }

    m_model->clear();

    QString remotePath = QUrl(m_remotePath).toLocalFile();
    if (remotePath.indexOf("//") == 0) {
        remotePath = remotePath.mid(2);
    }
    machinetalk::File *fileEntry =  m_tx.mutable_file_service_data()->add_files();
    fileEntry->set_name(remotePath.toStdString());
    this->sendFileLs(m_tx);
}

void ApplicationFile2::removeFile(const QString &name)
{
    if (!m_connected || (m_transferState != NoTransfer))
    {
        return;
    }

    machinetalk::File *fileEntry =  m_tx.mutable_file_service_data()->add_files();
    fileEntry->set_name(name.toStdString());
    this->sendFileDelete(m_tx);
}

void ApplicationFile2::createDirectory(const QString &name)
{
    if (!m_connected || (m_transferState != NoTransfer))
    {
        return;
    }

    machinetalk::File *fileEntry =  m_tx.mutable_file_service_data()->add_files();
    fileEntry->set_name(name.toStdString());
    this->sendFileMkdir(m_tx);
}

void ApplicationFile2::abort()
{
    updateState(NoTransfer); // TODO: find out if this is valid
}

void ApplicationFile2::clearError()
{
    updateState(NoTransfer);
    updateError(NoError);
}

void ApplicationFile2::cleanupFile()
{
    if (m_file == nullptr)
    {
        return;
    }

    m_file->close();
    m_file->deleteLater();
    m_file = nullptr;
}

void ApplicationFile2::handleFileListingMessage(const machinetalk::Container &rx)
{
    for (int i = 0; i < rx.file_service_data().files_size(); ++i)
    {
        const machinetalk::File &fileEntry = rx.file_service_data().files(i);
        ApplicationFileItem *item;
        bool isDir;

        item = new ApplicationFileItem();
        isDir = fileEntry.is_dir();
        item->setDir(isDir);
        if (!isDir)
        {
            item->setSize(static_cast<int>(fileEntry.size()));
        }
        item->setName(QString::fromStdString(fileEntry.name()));
        item->setLastModified(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(fileEntry.last_modified()))); // TODO

        m_model->addItem(item);
    }

    emit refreshFinished();
}

void ApplicationFile2::handleFileDataMessage(const machinetalk::Container &rx)
{
    QByteArray data;

    if (rx.file_service_data().files_size() == 0)
    {
        return;
    }

    const machinetalk::File &fileEntry = rx.file_service_data().files(0);
    data = QByteArray(fileEntry.blob().data(), static_cast<int>(fileEntry.blob().size()));

    m_file->write(data);
    cleanupFile();

    emit downloadFinished();
}

QString ApplicationFile2::generateTempPath() const
{
    return QUrl::fromLocalFile(QString("%1/machinetalk-%2").arg(QDir::tempPath())
                               .arg(QCoreApplication::applicationPid())).toString();
}

void ApplicationFile2::cleanupTempPath()
{
    QDir dir(m_localPath);
    dir.removeRecursively();
}

QString ApplicationFile2::applicationFilePath(const QString &fileName, const QString &serverDirectory) const
{
    return QDir(QUrl(m_localPath).toLocalFile() + "/" + serverDirectory).filePath(fileName);
}

void ApplicationFile2::commandSucceded()
{
    m_transferState = NoTransfer;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::commandFailed()
{
    m_error = ServiceError;
    emit errorChanged(m_error);
    m_transferState = NoTransfer;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::updateState(ApplicationFile2::TransferState state)
{
    if (state != m_transferState)
    {
        m_transferState = state;
        emit transferStateChanged(m_transferState);
    }
}

void ApplicationFile2::updateError(ApplicationFile2::TransferError error)
{
    if (m_error != error)
    {
        m_error = error;
        emit errorChanged(m_error);
    }
}

void ApplicationFile2::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void ApplicationFile2::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

void ApplicationFile2::getCmdStarted()
{
    m_transferState = DownloadRunning;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::getCmdSucceded()
{
    commandSucceded();
}

void ApplicationFile2::getCmdFailed()
{
    cleanupFile();
    commandFailed();
}

void ApplicationFile2::putCmdStarted()
{
    m_transferState = UploadRunning;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::putCmdSucceded()
{
    commandSucceded();
    emit uploadFinished();
}

void ApplicationFile2::putCmdFailed()
{
    commandFailed();
}

void ApplicationFile2::lsCmdStarted()
{
    m_transferState = RefreshRunning;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::lsCmdSucceded()
{
    commandSucceded();
}

void ApplicationFile2::lsCmdFailed()
{
    commandFailed();
}

void ApplicationFile2::mkdirCmdStarted()
{
    m_transferState = CreateDirectoryRunning;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::mkdirCmdSucceded()
{
    commandSucceded();
    emit createDirectoryFinished();
}

void ApplicationFile2::mkdirCmdFailed()
{
    commandFailed();
}

void ApplicationFile2::deleteCmdStarted()
{
    m_transferState = RemoveRunning;
    emit transferStateChanged(m_transferState);
}

void ApplicationFile2::deleteCmdSucceded()
{
    commandSucceded();
    emit removeFinished();
}

void ApplicationFile2::deleteCmdFailed()
{
    commandFailed();
}


} // namespace qtquickvcp
