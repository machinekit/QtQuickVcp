#include "filewatcher.h"
#include <QDebug>

namespace qtquickvcp {

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent)
    , m_fileUrl("")
    , m_enabled(true)
{
    connect(this, &FileWatcher::fileUrlChanged, this, &FileWatcher::updateWatchedFile);
    connect(&m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onWatchedFileChanged);
}

QUrl FileWatcher::fileUrl() const
{
    return m_fileUrl;
}

bool FileWatcher::enabled() const
{
    return m_enabled;
}

void FileWatcher::setFileUrl(const QUrl &fileUrl)
{
    if (m_fileUrl == fileUrl) {
        return;
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged(m_fileUrl);
}

void FileWatcher::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void FileWatcher::updateWatchedFile()
{
    for (const auto &file: m_fileSystemWatcher.files()) {
        m_fileSystemWatcher.removePath(file);
    }

    if (!m_fileUrl.isValid()) {
        return;
    }

    if (m_fileUrl.isLocalFile()) {
        m_fileSystemWatcher.addPath(m_fileUrl.toLocalFile());
    }
    else {
        qWarning() << "Can only watch local files";
    }
}

void FileWatcher::onWatchedFileChanged()
{
    if (m_enabled) {
        emit fileChanged();
    }
}

} // namespace qtquickvcp
