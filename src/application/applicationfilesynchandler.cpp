#include "applicationfilesynchandler.h"

namespace qtquickvcp {

ApplicationFileSyncHandler::ApplicationFileSyncHandler(QObject *parent)
    : QObject(parent)
    , m_remoteFilePath(QStringLiteral("file://"))
    , m_remotePath(QStringLiteral("file://"))
    , m_ignoreNextChange(false)
    , m_ready(false)
    , m_lastRemoteFilePath(QStringLiteral("file://"))
{
    connect(this, &ApplicationFileSyncHandler::remoteFilePathChanged, this, &ApplicationFileSyncHandler::checkRemoteFile);
    connect(this, &ApplicationFileSyncHandler::remotePathChanged, this, &ApplicationFileSyncHandler::checkRemoteFile);
    connect(this, &ApplicationFileSyncHandler::readyChanged, this, &ApplicationFileSyncHandler::checkRemoteFile);
}

QString ApplicationFileSyncHandler::remoteFilePath() const
{
    return m_remoteFilePath;
}

QString ApplicationFileSyncHandler::remotePath() const
{
    return m_remotePath;
}

bool ApplicationFileSyncHandler::ignoreNextChange() const
{
    return m_ignoreNextChange;
}

bool ApplicationFileSyncHandler::isReady() const
{
    return m_ready;
}

void ApplicationFileSyncHandler::setRemoteFilePath(const QString &remoteFilePath)
{
    if (m_remoteFilePath == remoteFilePath) {
        return;
    }

    m_remoteFilePath = remoteFilePath;
    emit remoteFilePathChanged(m_remoteFilePath);
}

void ApplicationFileSyncHandler::setRemotePath(const QString &remotePath)
{
    if (m_remotePath == remotePath) {
        return;
    }

    m_remotePath = remotePath;
    emit remotePathChanged(m_remotePath);
}

void ApplicationFileSyncHandler::setIgnoreNextChange(bool ignoreNextChange)
{
    if (m_ignoreNextChange == ignoreNextChange) {
        return;
    }

    m_ignoreNextChange = ignoreNextChange;
    emit ingoreNextChangeChanged(m_ignoreNextChange);
}

void ApplicationFileSyncHandler::setReady(bool ready)
{
    if (m_ready == ready) {
        return;
    }

    m_ready = ready;
    emit readyChanged(m_ready);
}

void ApplicationFileSyncHandler::checkRemoteFile()
{
    const auto remotePathUrl = QUrl(m_remotePath);
    const auto remoteFilePathUrl = QUrl(m_remoteFilePath);

    if (!remoteFilePathUrl.isLocalFile() || !remotePathUrl.isLocalFile()) {
        return;
    }

    if (!m_ready) {
        return;
    }

    if (remotePathUrl.toLocalFile() == QLatin1String("")) {
        return; // remote path is invalid
    }

    if (m_lastRemoteFilePath == m_remoteFilePath) {
        return; // filename did not change
    }

    if (remoteFilePathUrl.toLocalFile() == QLatin1String("")) {
        emit programUnloaded();
    }
    else if (remoteFilePathUrl.toLocalFile().indexOf(remotePathUrl.toLocalFile()) == 0) {
        m_lastRemoteFilePath = m_remoteFilePath;
        if (!m_ignoreNextChange) {
            emit startFileDownload(m_remoteFilePath);
        }
        else {
            m_ignoreNextChange = false;
        }
    }
    else {
        // subroutine call
    }
}
} // namespace qtquickvcp
