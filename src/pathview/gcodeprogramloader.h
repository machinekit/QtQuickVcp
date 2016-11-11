/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
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
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/

#ifndef GCODEPROGRAMLOADER_H
#define GCODEPROGRAMLOADER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include "gcodeprogrammodel.h"

namespace qtquickvcp {

class GCodeProgramLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localFilePath READ localFilePath WRITE setLocalFilePath NOTIFY localFilePathChanged)
    Q_PROPERTY(QString localPath READ localPath WRITE setLocalPath NOTIFY localPathChanged)
    Q_PROPERTY(QString remotePath READ remotePath WRITE setRemotePath NOTIFY remotePathChanged)
    Q_PROPERTY(GCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit GCodeProgramLoader(QObject *parent = 0);

    QString localFilePath() const
    {
        return m_localFilePath;
    }

    QString localPath() const
    {
        return m_localPath;
    }

    QString remotePath() const
    {
        return m_remotePath;
    }

    GCodeProgramModel * model() const
    {
        return m_model;
    }

signals:
    void localFilePathChanged(QString arg);
    void localPathChanged(QString arg);
    void remotePathChanged(QString arg);
    void modelChanged(GCodeProgramModel * arg);
    void loadingFinished();
    void loadingFailed();

public slots:
    void load();

    void setLocalFilePath(QString arg)
    {
        if (m_localFilePath != arg) {
            m_localFilePath = arg;
            emit localFilePathChanged(arg);
        }
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

    void setModel(GCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    QString m_localFilePath;
    QString m_localPath;
    QString m_remotePath;
    GCodeProgramModel * m_model;
}; // class GCodeProgramLoader
} // namespace qtquickvcp

#endif // GCODEPROGRAMLOADER_H
