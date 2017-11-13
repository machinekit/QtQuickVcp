/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

#include "filewatcher.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(loggingCategory, "filewatcher");

namespace qtquickvcp {

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent)
    , m_fileUrl("")
    , m_enabled(true)
    , m_recursive(false)
{
    connect(this, &FileWatcher::fileUrlChanged, this, &FileWatcher::updateWatchedFile);
    connect(this, &FileWatcher::enabledChanged, this, &FileWatcher::updateWatchedFile);
    connect(this, &FileWatcher::recursiveChanged, this, &FileWatcher::updateWatchedFile);
    connect(&m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onWatchedFileChanged);
    connect(&m_fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::onWatchedDirectoryChanged);
}

QUrl FileWatcher::fileUrl() const
{
    return m_fileUrl;
}

bool FileWatcher::enabled() const
{
    return m_enabled;
}

bool FileWatcher::recursive() const
{
    return m_recursive;
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
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void FileWatcher::setRecursive(bool recursive)
{
    if (m_recursive == recursive) {
        return;
    }

    m_recursive = recursive;
    emit recursiveChanged(m_recursive);
}

void FileWatcher::updateWatchedFile()
{
    const auto &files = m_fileSystemWatcher.files();
    if (files.length() > 0) {
        m_fileSystemWatcher.removePaths(files);
    }
    const auto &directories = m_fileSystemWatcher.directories();
    if (directories.length() > 0) {
        m_fileSystemWatcher.removePaths(directories);
    }

     if (!m_fileUrl.isValid() || !m_enabled) {
        return;
    }

    if (!m_fileUrl.isLocalFile()) {
        qCWarning(loggingCategory) << "Can only watch local files";
        return;
    }
    const auto &localFile = m_fileUrl.toLocalFile();
    if (localFile.isEmpty()) {
        return;
    }

    if (m_recursive && QDir(localFile).exists()) {
        QDirIterator it(localFile, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const auto &file = it.next();
            m_fileSystemWatcher.addPath(file);
        }
    }
    else if (QFile::exists(localFile)) {
        m_fileSystemWatcher.addPath(localFile);
    }
    else {
#ifdef QT_DEBUG
        qCWarning(loggingCategory) << "File to watch does not exist" << localFile;
#endif
    }
}

void FileWatcher::onWatchedFileChanged()
{
    if (m_enabled) {
        emit fileChanged();
    }
}

void FileWatcher::onWatchedDirectoryChanged(const QString &)
{
    updateWatchedFile();
    onWatchedFileChanged(); // propagate event
}

} // namespace qtquickvcp
