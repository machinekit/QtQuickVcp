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

#ifndef QAPPLICATIONDESCRIPTION_H
#define QAPPLICATIONDESCRIPTION_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QSettings>

class QApplicationDescription : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl sourceDir READ sourceDir WRITE setSourceDir NOTIFY sourceDirChanged)
    Q_PROPERTY(QUrl mainFile READ mainFile NOTIFY mainFileChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description  NOTIFY descriptionChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

public:
    explicit QApplicationDescription(QObject *parent = 0);

    QUrl sourceDir() const
    {
        return m_sourceDir;
    }

    QUrl mainFile() const
    {
        return m_mainFile;
    }

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }
    bool isValid() const
    {
        return m_valid;
    }

public slots:
    void setSourceDir(QUrl arg)
    {
        if (m_sourceDir != arg) {
            m_sourceDir = arg;
            emit sourceDirChanged(arg);
            m_valid = refresh();
            emit validChanged(m_valid);
        }
    }

private:
    bool m_componentCompleted;
    QUrl m_sourceDir;
    QUrl m_mainFile;
    QString m_name;
    QString m_description;
    bool m_valid;

    bool refresh();
    QString getMainFile(QStringList fileList, QString baseFilePath, QString applicationName);

signals:
    void sourceDirChanged(QUrl arg);
    void mainFileChanged(QUrl arg);
    void nameChanged(QString arg);
    void descriptionChanged(QString arg);
    void validChanged(bool arg);
};

#endif // QAPPLICATIONDESCRIPTION_H
