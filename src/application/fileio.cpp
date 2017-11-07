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

#include "fileio.h"
#include <QFile>
#include <QTextStream>

namespace qtquickvcp {

FileIO::FileIO(QObject *parent) :
    QObject(parent),
    m_fileUrl(""),
    m_text(""),
    m_working(false),
    m_temporaryDir(nullptr)
{

}

FileIO::~FileIO()
{
}

QUrl FileIO::fileUrl() const
{
    return m_fileUrl;
}

QString FileIO::text() const
{
    return m_text;
}

bool FileIO::working() const
{
    return m_working;
}

/* Write text to file */
void FileIO::write()
{

    if (!m_fileUrl.isValid() || !m_fileUrl.isLocalFile())
    {
        emit error(tr("File url is not valid"));
        return;
    }

    QString fileName = m_fileUrl.toLocalFile();
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        emit error(tr("Cannot open file"));
        return;
    }

    setWorking(true);
    QTextStream out(&file);
    out << m_text;
    file.close();
    setWorking(false);

    emit writingCompleted();
}

/* Read text from file */
void FileIO::read()
{
    if (!m_fileUrl.isValid() || !m_fileUrl.isLocalFile())
    {
        emit error(tr("File url is not valid"));
        return;
    }

    QString fileName = m_fileUrl.toLocalFile();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        emit error(tr("Cannot open file"));
        return;
    }

    setWorking(true);
    QTextStream in(&file);
    m_text = in.readAll();
    file.close();
    setWorking(false);

    emit textChanged(m_text);
    emit readingCompleted();
}

void FileIO::setFileUrl(const QUrl &fileUrl)
{
    if (m_fileUrl == fileUrl) {
        return;
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged(fileUrl);
}

void FileIO::setText(const QString &text)
{
    if (m_text == text) {
        return;
    }

    m_text = text;
    emit textChanged(text);
}

QUrl FileIO::createTempFile(const QString &fileName)
{
    if (!m_temporaryDir)
    {
        m_temporaryDir = std::make_unique<QTemporaryDir>();
        m_temporaryDir->setAutoRemove(true);
    }

    if (!m_temporaryDir->isValid())
    {
        return QUrl();
    }

    return QUrl::fromLocalFile(m_temporaryDir->path() + "/" + fileName);
}

void FileIO::setWorking(bool working)
{
    if (m_working == working)
        return;

    m_working = working;
    emit workingChanged(working);
}

} // namespace qtquickvcp
